#include <queue>
#include <forward_list>
#include <cassert>
#include <algorithm>

#include "Visioning.h"
#include "Utility.h"
#include "Vector.h"
#include "Intersections.h"
#include "Constants.h"
#include "JSONDataNode.h"

Visioning::Visioning(const DataNode &dataNode) :
    Entity(dataNode) , views({{{0, 0}, {0, 0}, {0, 0}}}) ,
    viewShape(std::make_unique<sf::VertexArray>(sf::TrianglesFan)) ,
    fieldOfView(dataNode.getNumber("fov")) , viewLength(dataNode.getNumber("view length"))
{
    updateViewVectors();
}

Visioning::Visioning(const Visioning &vis) :
    Entity(vis) , views({{{0, 0}, {0, 0}, {0, 0}}}) ,
    viewShape(std::make_unique<sf::VertexArray>(sf::TrianglesFan)) ,
    fieldOfView(vis.fieldOfView) , viewLength(vis.viewLength)
{}

void Visioning::updateViewShape(const Polygon::Array &polygons) {
    // reset edges array
    blockingEdges.clear();

    // resulting point array
    PointsVector_t hitPoints = collectViewPoints(polygons);

    //clear space from used points
    arcPoints.clear();
    endPoints.clear();

    // get rid of points at the same angle from center (store other points in different array)
    PointsVector_t resPoints = toRespoints(hitPoints);

    // clear used array
    hitPoints.clear();

    // finally set view shape
    setViewShape(resPoints);
}

void Visioning::updateViewVectors() {
    views = {{{(viewLength * std::cos(getAngle() + fieldOfView)), (viewLength * std::sin(getAngle() + fieldOfView))},
             {(viewLength * std::cos(getAngle())), (viewLength * std::sin(getAngle()))},
             {(viewLength * std::cos(getAngle() - fieldOfView)), (viewLength * std::sin(getAngle() - fieldOfView))}}};
}

Visioning::PointsVector_t Visioning::collectViewPoints(const Polygon::Array &polygons) {
    //points for bbox intersection
    Point topRight = getPosition() + Vector(viewLength, viewLength);
    Point bottomLeft = getPosition() - Vector(viewLength, viewLength);

    //set points and blocking edges array
    for (const auto &pol : *polygons) {
        //check if polygon not far away
        if (pol->bboxColliding(bottomLeft, topRight)) {
            processPolygon(*pol);
        }
    }

    PointsVector_t hitPoints;
    hitPoints.reserve(2 + arcPoints.size() + endPoints.size());

    //generate resulting points using ray check and collect them into array
    for (int i = 0;i < 2;++i) {
        hitPoints.push_back(rayCheck(views[2 * i]));
    }

    for (const auto &point: arcPoints) {
        Vector pointVector(getPosition(), point);
        hitPoints.push_back(rayCheck(pointVector));
    }

    for (const auto &point: endPoints) {//create 2 auxiliary points
        Vector pointVector(getPosition(), point);
        hitPoints.push_back(rayCheck(pointVector));

        pointVector.rotate(Util::degToRad(1));
        if (!inSemicircle(views, pointVector)) {
            hitPoints.push_back(rayCheck(pointVector));
        }

        pointVector.rotate(Util::degToRad(-2));
        if (!inSemicircle(views, pointVector)) {
            hitPoints.push_back(rayCheck(pointVector));
        }
    }

    return hitPoints;
}

void Visioning::processPolygon(const Polygon &polygon) {
    // iterate through all pairs
    auto endIt = polygon.getPoints().end();
    auto previousIt = endIt - 1;
    auto currentIt = polygon.getPoints().begin();

    // store point types
    PointType previousType = getPointType(*previousIt);
    PointType currentType;

    while (currentIt != endIt) {
        currentType = getPointType(*currentIt);

        // save current point type for next iteration
        PointType currentTypeTemp = currentType;

        // type of current point should be larger than previous
        const Point *currentPointPtr = &*currentIt;
        const Point *previousPointPtr = &*previousIt;

        if (previousType > currentType) {
            std::swap(currentPointPtr, previousPointPtr);
            std::swap(currentType, previousType);
        }

        const Point &previousPoint = *previousPointPtr;
        const Point &currentPoint = *currentPointPtr;

        switch (previousType) {
            case within:
                switch (currentType) {
                    case within: {
                        //within - within
                        endPoints.push_back(previousPoint);
                        endPoints.push_back(currentPoint);
                        blockingEdges.emplace_back(previousPoint, currentPoint);
                        break;
                    }
                    case behind: {
                        //within - behind
                        endPoints.push_back(previousPoint);
//                        auto intersectPoint = Inter::lineVSBrokenLine<std::vector<Point>>(Edge(previousPoint, currentPoint), getPosition(), views[0], views[1]);
//                        if(!intersectPoint.empty()){
//                            edgePoints.push_back(intersectPoint.front());
//                        }else{
//                            edgePoints.push_back(position);
//                        }
                        blockingEdges.emplace_back(previousPoint, currentPoint);
                        break;
                    }
                    case semicircle: {
                        //within - semicircle
                        endPoints.push_back(previousPoint);
//                        std::vector<Point> intersectPoint =
//                                twoLinesVSLineIntersection(previousPoint, currentPoint, position, views);
//                        edgePoints.push_back(intersectPoint.front());
                        blockingEdges.emplace_back(previousPoint, currentPoint);
                        break;
                    }
                    case outside: {
                        //within - outside
                        endPoints.push_back(previousPoint);
                        std::vector<Point> intersectPoint =
                                Inter::arcVSLine(previousPoint, currentPoint, getPosition(), viewLength, views);
                        if (intersectPoint.empty()) {
//                            intersectPoint = twoLinesVSLineIntersection(previousPoint, currentPoint, position, views);
//                            edgePoints.push_back(intersectPoint.front());
                        } else {
                            arcPoints.push_back(intersectPoint.front());
                        }
                        blockingEdges.emplace_back(previousPoint, currentPoint);
                        break;
                    }
                    default:
                        assert(false && "reached first default label in polygon processing visioning method");
                }
                break;
            case behind:
                switch (currentType) {
                    case behind: {
                        //behind - behind
                        //prune
                        break;
                    }
                    case outside: {
                        //behind - outside
                        auto intersectPoint = Inter::lineVSBrokenLine<std::vector<Point>>(previousPoint, currentPoint, getPosition(), views[0], views[2]);
                        if (intersectPoint.empty()) {
                            intersectPoint = Inter::arcVSLine(previousPoint, currentPoint, getPosition(), viewLength, views);
                            if (intersectPoint.size() == 2) {
                                arcPoints.push_back(intersectPoint.front());
                                arcPoints.push_back(intersectPoint.back());
                                blockingEdges.emplace_back(previousPoint, currentPoint);
                            }
                        } else if (intersectPoint.size() == 1) {
//                            edgePoints.push_back(intersectPoint.front());
                            intersectPoint = Inter::arcVSLine(previousPoint, currentPoint, getPosition(), viewLength, views);
                            arcPoints.push_back(intersectPoint.front());
                            blockingEdges.emplace_back(previousPoint, currentPoint);
                        } else {
//                            edgePoints.push_back(intersectPoint.front());
//                            edgePoints.push_back(intersectPoint.back());
                            blockingEdges.emplace_back(previousPoint, currentPoint);
                        }
                        break;
                    }
                    case semicircle: {
                        //behind - semicircle
                        auto intersectPoint = Inter::lineVSBrokenLine<std::vector<Point>>(previousPoint, currentPoint, getPosition(), views[0], views[2]);
                        if (!intersectPoint.empty()) {
//                            edgePoints.push_back(intersectPoint.front());
//                            edgePoints.push_back(intersectPoint.back());
                            blockingEdges.emplace_back(previousPoint, currentPoint);
                        }
                        break;
                    }
                    default:
                        assert(false && "reached second default label in polygon processing visioning method");
                }
                break;
            case semicircle:
                switch (currentType) {
                    case semicircle: {
                        //semicircle - semicircle
                        auto intersectPoint = Inter::lineVSBrokenLine<std::vector<Point>>(previousPoint, currentPoint, getPosition(), views[0], views[2]);
                        if (!intersectPoint.empty()) {
//                            edgePoints.push_back(intersectPoint.front());
//                            edgePoints.push_back(intersectPoint.back());
                            blockingEdges.emplace_back(previousPoint, currentPoint);
                        }
                        break;
                    }
                    case outside: {
                        //semicircle - outside
                        auto intersectPoint = Inter::lineVSBrokenLine<std::vector<Point>>(previousPoint, currentPoint, getPosition(), views[0], views[2]);
                        if (intersectPoint.size() == 2) {
//                            edgePoints.push_back(intersectPoint.front());
//                            edgePoints.push_back(intersectPoint.back());
                            blockingEdges.emplace_back(previousPoint, currentPoint);
                        } else if (intersectPoint.size() == 1) {
//                            edgePoints.push_back(intersectPoint.front());
                            intersectPoint = Inter::arcVSLine(previousPoint, currentPoint, getPosition(), viewLength, views);
                            arcPoints.push_back(intersectPoint.front());
                            blockingEdges.emplace_back(previousPoint, currentPoint);
                        }
                        break;
                    }
                    default:
                        assert(false && "reached third default label in processPolygon visioning method");
                }
                break;
            case outside: {
                //outside - outside
                auto intersectPoint = Inter::lineVSBrokenLine<std::vector<Point>>(previousPoint, currentPoint, getPosition(), views[0], views[2]);
                if (intersectPoint.empty()) { // 2 on arc
                    intersectPoint = Inter::arcVSLine(previousPoint, currentPoint, getPosition(), viewLength, views);
                    if (intersectPoint.size() == 2) {
                        arcPoints.push_back(intersectPoint.front());
                        arcPoints.push_back(intersectPoint.back());
                        blockingEdges.emplace_back(previousPoint, currentPoint);
                    }
                } else if (intersectPoint.size() == 1) { // 1 on edge, 1 on arc
//                    edgePoints.push_back(intersectPoint.front());
                    intersectPoint = Inter::arcVSLine(previousPoint, currentPoint, getPosition(), viewLength, views);
                    if (intersectPoint.size() == 1) {
                        arcPoints.push_back(intersectPoint.front());
                        blockingEdges.emplace_back(previousPoint, currentPoint);
                    }
                } else {//2 on edges
//                    edgePoints.push_back(intersectPoint.front());
//                    edgePoints.push_back(intersectPoint.back());
                    blockingEdges.emplace_back(previousPoint, currentPoint);
                }
                break;
            }
        }

        previousType = currentTypeTemp;
        previousIt = currentIt;
        ++currentIt;
    }
}

Visioning::PointType Visioning::getPointType(const Point &point) {
    Vector pointVector(getPosition(), point);

    if (views[1] * pointVector < 0) {
        return behind;
    } else if (pointVector.sqr() > pow(viewLength, 2)) {
        return outside;
    } else if (inSemicircle(views, pointVector)) {
        return semicircle;
    } else {
        return within;
    }
}

bool Visioning::inSemicircle(const Views_t &views, const Vector &vector){
    return (views[0].cross(vector) * vector.cross(views[2]) < 0);
}

Point Visioning::rayCheck(const Vector &pointVector) {
    double maxR_2 = viewLength * viewLength;

    std::unique_ptr<Point> hitPoint = nullptr;
    for(const auto &edge : blockingEdges){
        std::unique_ptr<Point> intersection = Inter::infLineVSLine(getPosition(), pointVector, edge.first, Vector(edge.first, edge.second));

        if(intersection != nullptr){
            Vector v(getPosition(), *intersection);

            if(v.sqr() < maxR_2){
                hitPoint = std::move(intersection);
                maxR_2 = v.sqr();
            }
        }
    }

    if(hitPoint != nullptr){
        Vector res(getPosition(), *hitPoint);

        if(res.sqr() > viewLength * viewLength) {
            double length = res.length();
            res.increase(viewLength / length);

            return getPosition() + res;
        } else{
            return *hitPoint;
        }
    }else{
        Vector res(pointVector);
        double length = res.length();
        res.increase(viewLength / length);
        return getPosition() + res;
    }
}

Visioning::PointsVector_t Visioning::toRespoints(PointsVector_t &hitPoints) const {
    //sort hitPoints by angle
    std::sort(std::begin(hitPoints), std::end(hitPoints), [this](const Point &p1, const Point &p2) -> bool {
        Vector v1(getPosition(), p1);
        Vector v2(getPosition(), p2);

        return v1.cross(v2) > 0;
    });

    // get rid of points at the same angle from center (store other points in different array)
    std::vector<Point> resPoints;
    for (auto it = hitPoints.begin(), prevIt = hitPoints.end(), endIt = hitPoints.end(); it < endIt; ++it) {
        Vector itVector(getPosition(), *it);

        if (prevIt == hitPoints.end() || !itVector.angleEqual(Vector(getPosition(), *prevIt))) {
            resPoints.push_back(*it);
        }

        prevIt = it;
    }

    return resPoints;
}

void Visioning::updateSeeingEntities(const EntityStorage &entityStorage) {
    auto entities = entityStorage.iterateEntities();
    for (const auto &entityPair : entities) {
        auto &entity = entityPair.second;
        bool visible = false;
        Vector enemyVector(getPosition(), entity.getPosition());

        if (enemyVector.sqr() < viewLength * viewLength && views[1] * enemyVector > 0 && !inSemicircle(views, enemyVector)) {
            if(!Inter::edgeVSEdgesCheck(Edge(getPosition(), entity.getPosition()), blockingEdges)){
                visible = true;
            }
        }

        entity.setVisibility(visible);
    }
}

void Visioning::setAngle(double angle_) {
    Entity::setAngle(angle_);

    updateViewVectors();
}

void Visioning::setViewShape(Visioning::PointsVector_t &resPoints) {
    // set first vertex
    viewShape->resize(1);
    setVertex((*viewShape)[0], getPosition().toSfVector());

    bool prevLiesOnArc = Vector(getPosition(), resPoints.front()).lengthEqual(viewLength);
    bool usedArc;

    for(auto curIt = ++resPoints.begin(), prevIt = resPoints.begin(), endIt = resPoints.end(); curIt < endIt; ++curIt, ++prevIt){
        usedArc = false;
        bool itLiesOnArc = Vector(getPosition(), *curIt).lengthEqual(viewLength);

        if(prevLiesOnArc && itLiesOnArc){
            std::unique_ptr<Edge> closestEdge = findClosestBlockingEdge(*curIt);

            if(!(closestEdge != nullptr && Vector(closestEdge->first, closestEdge->second).angleEqual(Vector(*curIt, *prevIt)))){
                Vector prevVector(getPosition(), *prevIt);
                Vector itVector(getPosition(), *curIt);
                Vector zeroVector(1, 0);

                double prevAngle = prevVector.y > 0 ?
                                   std::acos((zeroVector * prevVector) / viewLength) :
                                   2 * PI_VALUE - std::acos((zeroVector * prevVector) / viewLength);
                double itAngle = itVector.y > 0 ?
                                 std::acos((zeroVector * itVector) / viewLength) :
                                 2 * PI_VALUE - std::acos((zeroVector * itVector) / viewLength);

                // set view sector as arc
                addViewSector(itAngle, prevAngle);
                usedArc = true;
            }
        }

        // if it wasn't arc set as edge
        if(!usedArc){
            addEdgeSector(*prevIt, *curIt);
        }
        prevLiesOnArc = itLiesOnArc;
    }
}

std::unique_ptr<Edge> Visioning::findClosestBlockingEdge(const Point &point){
    if(blockingEdges.empty()){
        return nullptr;
    }

    double minDistSqr = Vector(point, calcClosestPoint(blockingEdges.front(), point)).sqr();
    auto edgeIt = blockingEdges.begin();

    for(auto it = ++blockingEdges.begin(), endIt = blockingEdges.end();it != endIt;++it){
        double distSqr = Vector(point, calcClosestPoint(*it, point)).sqr();

        if(distSqr < minDistSqr){
            edgeIt = it;
            minDistSqr = distSqr;
        }
    }

    return std::make_unique<Edge>(*edgeIt);
}

Point Visioning::calcClosestPoint(const Edge &edge, const Point &point) {
    if (edge.first.x == edge.second.x){
        return { edge.first.x, point.y };
    }

    if (edge.first.y == edge.second.y){
        return { point.x, edge.first.x };
    }

    double m1 = (edge.second.y - edge.first.y) / (edge.second.x - edge.first.x);
    double m2 = -1 / m1;
    double x = (m1 * edge.first.x - m2 * point.x + point.y - edge.first.y) / (m1 - m2);

    return { x, m2 * (x - point.x) + point.y };
}

void Visioning::addViewSector(double currentAngle, double viewAngle){
    if(viewAngle > currentAngle){
        viewAngle -= 2 * PI_VALUE;
    }
    viewAngle = currentAngle - viewAngle;

    int dotsAmount = (int) (viewLength * viewAngle); // dots in arc

    if(dotsAmount < 1){
        return;
    }

    size_t index = viewShape->getVertexCount();
    size_t topIndex = index + dotsAmount;
    viewShape->resize(topIndex);
    double dAngle = viewAngle / (double) (dotsAmount + 1);

    for(;index < topIndex;++index){
        setVertex((*viewShape)[index], sf::Vector2f((float) (getPosition().x + viewLength * std::cos(currentAngle)), (float) (getPosition().y + viewLength * std::sin(currentAngle))));

        currentAngle -= dAngle;
    }
}

void Visioning::addEdgeSector(const Point &p1, const Point &p2) {
    size_t index = viewShape->getVertexCount();
    viewShape->resize(index + 2);

    setVertex((*viewShape)[index], p1.toSfVector());
    setVertex((*viewShape)[index + 1], p2.toSfVector());
}

void Visioning::drawViewShape(WindowHolder &window, const sf::Texture &texture) const {
    // should i use visibility here?
    window.draw(*viewShape, &texture);
}

std::string Visioning::serialize() const {
    using Formatter = JSONDataNode;
    return Formatter::formatNumber(FOVKEY, fieldOfView) + "," +
           Formatter::formatNumber(VIEWLENGTHKEY, viewLength);
}

void Visioning::setVertex(sf::Vertex &vertex, const sf::Vector2f &vector) {
    vertex.position = vector;
    vertex.texCoords = vector;
}
