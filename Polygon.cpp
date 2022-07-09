#include "Polygon.h"
#include "Player.h"
#include <iostream>

Polygon::Polygon() = default;

void Polygon::addBPoint(const PolygonPoint &point) {
	boarderPoints.push_back(point);
	
	if(boarderPoints.size() > 1){
		connectBPoints(*std::prev(boarderPoints.end(), 2), *std::prev(boarderPoints.end(), 1));
	}
}

void Polygon::endAdding() {
	if(boarderPoints.size() > 1){
		connectBPoints(boarderPoints.back(), boarderPoints.front());
	}
}

void Polygon::connectBPoints(const PolygonPoint &bp1, const PolygonPoint &bp2) {
	int ratio = (int) (bp1.getDistance(bp2) / minDistance) + 1;//amount of line segments
	
	if (ratio > 1) {
		float width = std::abs(bp1.x - bp2.x) / (float) ratio, height = std::abs(bp1.y - bp2.y) / (float) ratio;
		
		float dx = bp1.x < bp2.x ? 1 : -1, dy = bp1.y < bp2.y ? 1 : -1;
		--ratio;//dots amount = line segments amount - 1
		for (int i = 0; i < ratio; ++i) {
			extraPoints.emplace_back(*this, bp1.x + width * (float) (i + 1) * dx, bp1.y + height * (float) (i + 1) * dy);
		}
	}
}

void Polygon::setConvex() {
	convex = std::make_shared<sf::ConvexShape>();
	
	convex->setPointCount(boarderPoints.size());
	convex->setFillColor(sf::Color(0, 0, 0, 200));
	
	int i = 0;
	for(const auto &point : boarderPoints){
		convex->setPoint(i++, sf::Vector2f(point.x, point.y));
	}
}

void Polygon::makeInvisible() {
	visible = false;
}

void Polygon::makeVisible() {
	visible = true;
}

void Polygon::setPointsType(const Player &player, const std::array<Vector, 3> &views) {
    const Point &zeroPoint = player.getPosition();
    float radius = player.getViewDistance();

    for (auto &point : boarderPoints) {
        Vector pointVector(zeroPoint, point);

        if (views[1] * pointVector < 0) {
            point.type = behind;
        } else if (pointVector.sqr() > std::pow(radius, 2)) {
            point.type = outside;
        } else if (!isOutOfBoarders(pointVector, views)) {
            point.type = within;
        } else {
            point.type = semicircle;
        }
    }


}

void Polygons::add(std::shared_ptr<Polygon> &polygon) {
	polygons.push_back(std::move(polygon));
	
	for(const auto &it : polygons.back()->boarderPoints){
		points.push_back(it);
	}
	
	for(const auto &it : polygons.back()->extraPoints){
		points.push_back(it);
	}
}

std::vector<std::shared_ptr<sf::Shape>> Polygons::collectShapes() const {
	std::vector<std::shared_ptr<sf::Shape>> shapes;
	
	for(const auto &it : polygons){
		shapes.push_back(it->convex);
	}
	
	return shapes;
}

void Polygons::hideAll() {
	for(auto &it : polygons){
		it->makeInvisible();
	}
}

PolygonPoint &Polygons::operator[](unsigned int index) {
	return points[index];
}

std::vector<Point> arcSegmentVSLineIntersection(const Point &p1, const Point &p2, const Point &center, float radius, const std::array<Vector, 3> &views){
    //|X^2 - center^2| = radius^2
    //X(t) = p1 + td , (t from 0 to 1)
    //delta = p1 - center
    Vector d(p1, p2);
    Vector delta(center, p1);

    std::vector<Point> res;

    auto isInSight = [&](const Point &point) -> bool {
        Vector pV(center, point);
        if(pV * views[1] < 0){
            return false;
        }else if(views[0].cross(pV) * pV.cross(views[2]) > 0){
            return true;
        }else{
            return false;
        }
    };

    float discriminant = (d * delta) * (d * delta) - d.sqr() * (delta.sqr() - radius * radius);

    if(discriminant < 0){
        return res;
    }else if(discriminant == 0){
        float t = (-1 * (d * delta)) / (d.sqr());
        if(t >= 0 && t <= 1){
            Point p(p1 + t * d);
            if(isInSight(p)){
                res.push_back(p);
            }
        }
        return res;
    }else{
        discriminant = std::sqrt(discriminant);

        float t;
        t = ((-1 * (d * delta)) + discriminant) / (d.sqr());
        if(t >= 0 && t <= 1){
            Point p(p1 + t * d);
            if(isInSight(p)){
                res.push_back(p);
            }
        }
        t = ((-1 * (d * delta)) - discriminant) / (d.sqr());
        if(t >= 0 && t <= 1){
            Point p(p1 + t * d);
            if(isInSight(p)){
                res.push_back(p);
            }
        }

        return res;
    }
}

std::vector<Point> twoLinesVSLineIntersection(const Point &p1, const Point &p2, const Point &center, const std::array<Vector, 3> &views){
    //1 line: p1 + s * d0
    const Vector d0(p1, p2);
    //2 line: center + t * views[0]
    //3 line: center + t * views[2]
    //p1 == P0 ; center == P1

    const Vector delta(p1, center);

    std::vector<Point> res;

    for(int i = 0;i < 3;i += 2){//for views[0] and views[2]
        const Vector &d1 = views[i];
        float denominator = d0 * (d1.perpendicular());

        if(denominator != 0){
            float t = (delta * (d0.perpendicular())) / denominator;
            float s = (delta * (d1.perpendicular())) / denominator;
            if(t >= 0 && t <= 1 && s >= 0 && s <= 1){
                Point cross = center + (t * d1);
                res.emplace_back(cross);
            }
        }
    }

    return res;
}

std::vector<Point> lineVSLineIntersection(const Point &p1, const Vector &d0, const Point &p3, const Vector &d1){//d0 is unlimited
    const Vector delta(p1, p3);

    std::vector<Point> res;

    float denominator = d0 * (d1.perpendicular());

    if(denominator != 0){
        float t = (delta * (d0.perpendicular())) / denominator;
        float s = (delta * (d1.perpendicular())) / denominator;
        if(t >= 0 && t <= 1){
            Point cross = p3 + (t * d1);
            res.emplace_back(cross);
        }
    }

    return res;
}

#ifdef T5_DEBUG
void drawPoint_DBG(sf::RenderWindow &window, PointType type, const Point &point){
    sf::CircleShape circle(4);
    circle.setOrigin(4, 4);
    circle.setPosition(point.x, point.y);

    switch(type){
        case within: circle.setFillColor(sf::Color::Green); break;
        case semicircle: circle.setFillColor(sf::Color::Red); break;
        case outside: circle.setFillColor(sf::Color::Magenta); break;
        case behind: circle.setFillColor(sf::Color::Black); break;
    }

    window.draw(circle);
}
#endif

Point rayCheck(const Point &center, float radius, const Vector &pointVector, const std::vector<Edge> &blockingEdges){
    float maxR = radius * radius;
    bool hit = false;
    Point hitPoint(0, 0);
    for(const auto &edge : blockingEdges){
        std::vector<Point> intersection = lineVSLineIntersection(center, pointVector, edge.first, Vector(edge.first, edge.second));
        if(!intersection.empty()){
            Vector v(center, intersection.front());
            if(v.sqr() < maxR){
                hitPoint = intersection.front();
                hit = true;
                maxR = v.sqr();
            }
        }
    }

    if(hit){
        Vector res(center, hitPoint);
        if(res.sqr() > radius * radius) {
            float length = res.length();
            res.increase(radius / length);
            return center + res;
        }
    }else{
        Vector res(pointVector);
        float length = res.length();
        res.increase(radius / length);
        return center + res;
    }

    return hitPoint;
}

bool isOutOfBoarders(const Vector &pointVector, const std::array<Vector, 3> &views){
    return (views[0].cross(pointVector) * pointVector.cross(views[2]) < 0);
}


void Polygons::updateVisibility(Player &player
#ifdef T5_DEBUG
                                , sf::RenderWindow &window
#endif
                                ) {
    const Point &center = player.getPosition();
    float radius = player.getViewDistance();
    float angle = player.getAngle();
    float diffAngle = player.getViewAngle();

    std::array<Vector, 3> views{{  {(float) (radius * cos(angle + diffAngle)), (float) (radius * sin(angle + diffAngle))},
                                   {(float) (radius * cos(angle)), (float) (radius * sin(angle))},
                                   {(float) (radius * cos(angle - diffAngle)), (float) (radius * sin(angle - diffAngle))}}};

    //set points type:
    for(const auto &pol : polygons) {
        if(pol->visible){
            pol->setPointsType(player, views);
        }
    }

    //result arrays:
    std::vector<Edge> blockingEdges;
    std::vector<Point> arcPoints, edgePoints, endPoints;

    //set angle points and block edges array
    for(const auto &pol : polygons) {
        if(!pol->visible){
            continue;
        }

        const Point *previousPointPtr = &pol->boarderPoints.back();
        PointType previous = pol->boarderPoints.back().type, current;

        for(const auto &point : pol->boarderPoints){
            current = point.type;
            PointType currentTemp = current;

            const Point *currentPointPtr = &point;

            if(previous > current){
                std::swap(currentPointPtr, previousPointPtr);
                std::swap(current, previous);
            }

            const Point &currentPoint = *currentPointPtr;
            const Point &previousPoint = *previousPointPtr;

            switch (previous){
                case within:
                    switch (current){
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
                            std::vector<Point> intersectPoint = twoLinesVSLineIntersection(previousPoint, currentPoint,
                                                                                           center, views);
                            edgePoints.push_back(intersectPoint.front());
                            blockingEdges.emplace_back(previousPoint, currentPoint);
                            break;
                        }
                        case semicircle: {
                            //within - semicircle
                            endPoints.push_back(previousPoint);
                            std::vector<Point> intersectPoint = twoLinesVSLineIntersection(previousPoint, currentPoint,
                                                                                           center, views);
                            edgePoints.push_back(intersectPoint.front());
                            blockingEdges.emplace_back(previousPoint, currentPoint);
                            break;
                        }
                        case outside: {
                            //within - outside
                            endPoints.push_back(previousPoint);
                            std::vector<Point> intersectPoint = arcSegmentVSLineIntersection(previousPoint, currentPoint,
                                                                                             center, radius, views);
                            if (intersectPoint.empty()) {
                                intersectPoint = twoLinesVSLineIntersection(previousPoint, currentPoint, center, views);
                                edgePoints.push_back(intersectPoint.front());
                            }
                            else{
                                arcPoints.push_back(intersectPoint.front());
                            }
                            blockingEdges.emplace_back(previousPoint, currentPoint);
                            break;
                        }
                    }
                    break;
                case behind:
                    switch (current){
                        case behind: {
                            //behind - behind
                            //prune
                            break;
                        }
                        case outside: {
                            //behind - outside
                            std::vector<Point> intersectPoint = twoLinesVSLineIntersection(previousPoint, currentPoint, center, views);
                            if (intersectPoint.empty()) {
                                intersectPoint = arcSegmentVSLineIntersection(previousPoint, currentPoint, center, radius, views);
                                if(intersectPoint.size() == 2){
                                    arcPoints.push_back(intersectPoint.front());
                                    arcPoints.push_back(intersectPoint.back());
                                    blockingEdges.emplace_back(previousPoint, currentPoint);
                                }
                            } else if(intersectPoint.size() == 1) {
                                edgePoints.push_back(intersectPoint.front());
                                intersectPoint = arcSegmentVSLineIntersection(previousPoint, currentPoint, center, radius, views);
                                arcPoints.push_back(intersectPoint.front());
                                blockingEdges.emplace_back(previousPoint, currentPoint);
                            } else{
                                edgePoints.push_back(intersectPoint.front());
                                edgePoints.push_back(intersectPoint.back());
                                blockingEdges.emplace_back(previousPoint, currentPoint);
                            }
                            break;
                        }
                        case semicircle: {
                            //behind - semicircle
                            std::vector<Point> intersectPoint = twoLinesVSLineIntersection(previousPoint, currentPoint,
                                                                                           center, views);
                            if(!intersectPoint.empty()){
                                edgePoints.push_back(intersectPoint.front());
                                edgePoints.push_back(intersectPoint.back());
                                blockingEdges.emplace_back(previousPoint, currentPoint);
                            }
                            break;
                        }
                    }
                    break;
                case semicircle:
                    switch (current){
                        case semicircle: {
                            //semicircle - semicircle
                            std::vector<Point> intersectPoint = twoLinesVSLineIntersection(previousPoint, currentPoint, center, views);
                            if (!intersectPoint.empty()) {
                                edgePoints.push_back(intersectPoint.front());
                                edgePoints.push_back(intersectPoint.back());
                                blockingEdges.emplace_back(previousPoint, currentPoint);
                            }
                            break;
                        }
                        case outside:
                            //semicircle - outside
                            std::vector<Point> intersectPoint = twoLinesVSLineIntersection(previousPoint, currentPoint, center, views);
                            if (intersectPoint.size() == 2) {
                                edgePoints.push_back(intersectPoint.front());
                                edgePoints.push_back(intersectPoint.back());
                                blockingEdges.emplace_back(previousPoint, currentPoint);
                            } else if(intersectPoint.size() == 1){
                                edgePoints.push_back(intersectPoint.front());
                                intersectPoint = arcSegmentVSLineIntersection(previousPoint, currentPoint, center, radius, views);
                                arcPoints.push_back(intersectPoint.front());
                                blockingEdges.emplace_back(previousPoint, currentPoint);
                            }
                            break;
                    }
                    break;
                case outside: {
                    //outside - outside
                    std::vector<Point> intersectPoint = twoLinesVSLineIntersection(previousPoint, currentPoint, center, views);
                    if (intersectPoint.empty()) {//2 on arc
                        intersectPoint = arcSegmentVSLineIntersection(previousPoint, currentPoint, center, radius, views);
                        if (intersectPoint.size() == 2) {
                            arcPoints.push_back(intersectPoint.front());
                            arcPoints.push_back(intersectPoint.back());
                            blockingEdges.emplace_back(previousPoint, currentPoint);
                        }
                    } else if (intersectPoint.size() == 1) {//1 on edge, 1 on arc
                        edgePoints.push_back(intersectPoint.front());
                        intersectPoint = arcSegmentVSLineIntersection(previousPoint, currentPoint, center, radius, views);
                        if (intersectPoint.size() == 1) {
                            arcPoints.push_back(intersectPoint.front());
                            blockingEdges.emplace_back(previousPoint, currentPoint);
                        }
                    } else {//2 on edges
                        edgePoints.push_back(intersectPoint.front());
                        edgePoints.push_back(intersectPoint.back());
                        blockingEdges.emplace_back(previousPoint, currentPoint);
                    }

                    break;
                }
            }

            previous = currentTemp;
            previousPointPtr = &point;
        }
    }

#ifdef T5_DEBUG
//    std::cout << edgePoints.size() << ' ' << blockingEdges.size() << std::endl;
//    std::cout << blockingEdges.front().first.x << ' ' << blockingEdges.front().first.y << std::endl;

    sf::VertexArray lines(sf::LinesStrip, 2);
    lines[0].position = sf::Vector2f(center.x, center.y);
    lines[0].color = sf::Color::Red;
    lines[1].position = sf::Vector2f(views[0].x + center.x, views[0].y + center.y);
    lines[1].color = sf::Color::Red;
    window.draw(lines);

    lines = sf::VertexArray(sf::LinesStrip, 2);
    lines[0].position = sf::Vector2f(center.x, center.y);
    lines[0].color = sf::Color::Red;
    lines[1].position = sf::Vector2f(views[2].x + center.x, views[2].y + center.y);
    lines[1].color = sf::Color::Red;
    window.draw(lines);

//    std::shared_ptr<sf::ConvexShape> convex = std::make_shared<sf::ConvexShape>();
//    convex->setFillColor(sf::Color::Transparent);
//    convex->setOutlineColor(sf::Color::Red);
//    convex->setOutlineThickness(1);
//    convex->setPosition(center.x, center.y);
//    initViewSector(convex, radius, diffAngle * 2);
//    convex->setRotation(radToDeg(angle + M_PI_2));
//    window.draw(*convex);

    for(const auto &edge : blockingEdges){
        lines = sf::VertexArray(sf::LinesStrip, 2);
        lines[0].position = sf::Vector2f(edge.first.x, edge.first.y);
        lines[0].color = sf::Color::Red;
        lines[1].position = sf::Vector2f(edge.second.x, edge.second.y);
        lines[1].color = sf::Color::Red;
        window.draw(lines);

        sf::CircleShape circle(4);
        circle.setFillColor(sf::Color::Red);
        circle.setOrigin(4, 4);
        circle.setPosition(edge.first.x, edge.first.y);
        window.draw(circle);
        circle.setPosition(edge.second.x, edge.second.y);
        window.draw(circle);
    }
#endif

    //ray check
    std::vector<Point> hitPoints;
//    Point leftPoint, rightPoint;
//    float leftDistance = radius * radius, rightDistance = radius * radius;
//    for(const auto &point : edgePoints){
//        Vector pointVector(center, point);
//        if(views[1].cross(pointVector) > 0){//right
//            if(pointVector.sqr() < rightDistance){
//                rightPoint = point;
//                rightDistance = pointVector.sqr();
//            }
//        }else{//left
//            if(pointVector.sqr() < leftDistance){
//                leftPoint = point;
//                leftDistance = pointVector.sqr();
//            }
//        }
//    }
//
//    if(leftDistance != radius * radius){
//        hitPoints.push_back(leftPoint);
//    }
//    else{
//        hitPoints.push_back(center + views[2]);
//    }
//    if(rightDistance != radius * radius){
//        hitPoints.push_back(rightPoint);
//    }
//    else{
//        hitPoints.push_back(center + views[0]);
//    }

    for(int i = 0;i < 2;++i){
        Vector pointVector(center, center + views[2 * i]);
        hitPoints.push_back(rayCheck(center, radius, pointVector, blockingEdges));
    }

    for(const auto &point : arcPoints){
        Vector pointVector(center, point);
        hitPoints.push_back(rayCheck(center, radius, pointVector, blockingEdges));
    }
    for(const auto &point : endPoints){//create 2 auxiliary points
        Vector pointVector(center, point);
        hitPoints.push_back(rayCheck(center, radius, pointVector, blockingEdges));

        pointVector.rotate(degToRad(1));
        if(!isOutOfBoarders(pointVector, views)){
            hitPoints.push_back(rayCheck(center, radius, pointVector, blockingEdges));
        }

        pointVector.rotate(degToRad(-2));
        hitPoints.push_back(rayCheck(center, radius, pointVector, blockingEdges));
    }

#ifdef T5_DEBUG
    for(const auto &point : hitPoints){
        sf::CircleShape circle(4);
        circle.setFillColor(sf::Color::Green);
        circle.setOrigin(4, 4);
        circle.setPosition(point.x, point.y);
        window.draw(circle);
    }
#endif

    //sort hitPoints by angle
    std::sort(hitPoints.begin(), hitPoints.end(), [&center] (const Point &p1, const Point &p2) -> bool {
        Vector v1(center, p1);
        Vector v2(center, p2);

        return v1.cross(v2) > 0;
    });

    //get rid of points at the same angle from center
    std::vector<Point> resPoints;
    auto prevIt = hitPoints.end();
    for(auto it = hitPoints.begin(), endIt = hitPoints.end();it < endIt;++it){
        Vector itVector(center, *it);

        if (prevIt == hitPoints.end() || itVector.cross(Vector(center, *prevIt)) != 0) {
            resPoints.push_back(*it);
        }

        prevIt = it;
    }
    resPoints.push_back(center);

    //set convex
    std::shared_ptr<sf::ConvexShape> &convex = player.getViewShape();
    size_t convexSize = resPoints.size();
    std::cout << convexSize << std::endl;
    convex->setPointCount(convexSize);

    int i = 0;
    for(const auto &it : resPoints){
        convex->setPoint(i, sf::Vector2f(it.x, it.y));

        ++i;
    }
}
