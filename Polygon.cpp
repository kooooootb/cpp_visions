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
        } else if (views[0].cross(pointVector) * views[2].cross(pointVector) > 0) {
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
    //1 line: p1 + s * d1
    const Vector d1(p1, p2);
    const Vector d1L = d1.perpendicular();
    //2 line: center + t * views[0]
    //3 line: center + t * views[2]

    std::vector<Point> res;

    for(int i = 0;i < 3;i+=2){
        const Vector &d2 = views[i], d2L = d2.perpendicular();
        float denominator = d1 * d2L;

        if(denominator != 0){
            float t = Vector(p1, p2) * d1L / denominator;
            if(t >= 0 && t <= 1){
                res.emplace_back(center + t * d2);
            }
        }
    }

    return res;
}

void Polygons::updateVisibility(Player &player) {
    const Point &center = player.getPosition();
    float radius = player.getViewDistance();
    float angle = player.getAngle();
    float diffAngle = player.getViewAngle() / 2;

    std::array<Vector, 3> views{{  {(float) (radius * cos(angle + diffAngle)), (float) (radius * sin(angle + diffAngle))},
                                   {(float) (radius * cos(angle - diffAngle)), (float) (radius * sin(angle - diffAngle))},
                                   {(float) (radius * cos(angle)), (float) (radius * sin(angle))} }};

    //set points type:
    for(const auto &pol : polygons) {
        pol->setPointsType(player, views);
    }

    //result arrays:
    using Edge = std::pair<Point, Point>;
    std::vector<Edge> blockingEdges;
    std::vector<Point> anglePoints;

    //set angle points and block edges array
    for(const auto &pol : polygons) {
        const Point *previousPointPtr = &pol->boarderPoints.back();
        PointType previous = pol->boarderPoints.back().type, current;

        for(const auto &point : pol->boarderPoints){
            current = point.type;
            const Point *currentPointPtr = &point;

            if(previous > current){
                std::swap(currentPointPtr, previousPointPtr);
            }

            const Point &currentPoint = *currentPointPtr;
            const Point &previousPoint = *previousPointPtr;

            switch (previous){
                case within:
                    switch (current){
                        case within: {
                            //within - within
                            anglePoints.push_back(previousPoint);
                            blockingEdges.emplace_back(previousPoint, currentPoint);
                            break;
                        }
                        case behind: {
                            //within - behind
                            anglePoints.push_back(previousPoint);
                            std::vector<Point> intersectPoint = twoLinesVSLineIntersection(previousPoint, currentPoint,
                                                                                           center, views);
                            anglePoints.push_back(intersectPoint.front());
                            blockingEdges.emplace_back(previousPoint, currentPoint);
                            break;
                        }
                        case semicircle: {
                            //within - semicircle
                            anglePoints.push_back(previousPoint);
                            std::vector<Point> intersectPoint = twoLinesVSLineIntersection(previousPoint, currentPoint,
                                                                                           center, views);
                            anglePoints.push_back(intersectPoint.front());
                            blockingEdges.emplace_back(previousPoint, currentPoint);
                            break;
                        }
                        case outside: {
                            //within - outside
                            anglePoints.push_back(previousPoint);
                            std::vector<Point> intersectPoint = arcSegmentVSLineIntersection(previousPoint, currentPoint,
                                                                                             center, radius, views);
                            if (intersectPoint.empty()) {
                                intersectPoint = twoLinesVSLineIntersection(previousPoint, currentPoint, center, views);
                            }
                            anglePoints.push_back(intersectPoint.front());
                            blockingEdges.emplace_back(previousPoint, intersectPoint.front());
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
                            std::vector<Point> intersectPoint = arcSegmentVSLineIntersection(previousPoint, currentPoint,
                                                                                             center, radius, views);
                            if (intersectPoint.empty()) {
                                intersectPoint = twoLinesVSLineIntersection(previousPoint, currentPoint, center, views);
                                anglePoints.push_back(intersectPoint.front());
                                anglePoints.push_back(intersectPoint.back());
                            } else if(intersectPoint.size() == 1) {
                                anglePoints.push_back(intersectPoint.front());
                                intersectPoint = twoLinesVSLineIntersection(previousPoint, currentPoint, center, views);
                                anglePoints.push_back(intersectPoint.front());
                            } else{
                                anglePoints.push_back(intersectPoint.front());
                                anglePoints.push_back(intersectPoint.back());
                            }
                            blockingEdges.emplace_back(*std::prev(anglePoints.end(), 2), *std::prev(anglePoints.end(), 1));
                            break;
                        }
                        case semicircle: {
                            //behind - semicircle
                            std::vector<Point> intersectPoint = twoLinesVSLineIntersection(previousPoint, currentPoint,
                                                                                           center, views);
                            if(!intersectPoint.empty()){
                                anglePoints.push_back(intersectPoint.front());
                                anglePoints.push_back(intersectPoint.back());
                                blockingEdges.emplace_back(*std::prev(anglePoints.end(), 2), *std::prev(anglePoints.end(), 1));
                            }
                            break;
                        }
                    }
                    break;
                case semicircle:
                    switch (current){
                        case semicircle: {
                            //semicircle - semicircle
                            std::vector<Point> intersectPoint = twoLinesVSLineIntersection(previousPoint, currentPoint,
                                                                                           center, views);
                            if (!intersectPoint.empty()) {
                                anglePoints.push_back(intersectPoint.front());
                                anglePoints.push_back(intersectPoint.back());
                                blockingEdges.emplace_back(*std::prev(anglePoints.end(), 2), *std::prev(anglePoints.end(), 1));
                            }
                            break;
                        }
                        case outside:
                            //semicircle - outside
                            std::vector<Point> intersectPoint = twoLinesVSLineIntersection(previousPoint, currentPoint,
                                                                                           center, views);
                            if (intersectPoint.size() == 2) {
                                anglePoints.push_back(intersectPoint.front());
                                anglePoints.push_back(intersectPoint.back());
                                blockingEdges.emplace_back(*std::prev(anglePoints.end(), 2), *std::prev(anglePoints.end(), 1));
                            } else if(intersectPoint.size() == 1){
                                anglePoints.push_back(intersectPoint.front());
                                intersectPoint = arcSegmentVSLineIntersection(previousPoint, currentPoint, center, radius, views);
                                anglePoints.push_back(intersectPoint.front());
                                blockingEdges.emplace_back(*std::prev(anglePoints.end(), 2), *std::prev(anglePoints.end(), 1));
                            }
                            break;
                    }
                    break;
                case outside: {
                    //outside - outside
                    std::vector<Point> intersectPoint = twoLinesVSLineIntersection(previousPoint, currentPoint,
                                                                                   center, views);
                    if (intersectPoint.empty()) {//2 on arc
                        intersectPoint = arcSegmentVSLineIntersection(previousPoint, currentPoint,
                                                                      center, radius, views);
                        if (intersectPoint.size() == 2) {
                            anglePoints.push_back(intersectPoint.front());
                            anglePoints.push_back(intersectPoint.back());
                            blockingEdges.emplace_back(*std::prev(anglePoints.end(), 2),
                                                       *std::prev(anglePoints.end(), 1));
                        }
                    } else if (intersectPoint.size() == 1) {//1 on edge, 1 on arc
                        anglePoints.push_back(intersectPoint.front());
                        intersectPoint = arcSegmentVSLineIntersection(previousPoint, currentPoint,
                                                                      center, radius, views);
                        if (intersectPoint.size() == 1) {
                            anglePoints.push_back(intersectPoint.front());
                            blockingEdges.emplace_back(*std::prev(anglePoints.end(), 2),
                                                       *std::prev(anglePoints.end(), 1));
                        }
                    } else {//2 on edges
                        anglePoints.push_back(intersectPoint.front());
                        anglePoints.push_back(intersectPoint.back());
                        blockingEdges.emplace_back(*std::prev(anglePoints.end(), 2), *std::prev(anglePoints.end(), 1));
                    }

                    break;
                }
            }

            previous = current;
            previousPointPtr = &point;
        }
    }

    std::cout << anglePoints.size() << ' ' << blockingEdges.size() << std::endl;


}
