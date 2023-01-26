#include "Polygon.h"
#include "Player.h"
#include <utility>

Polygon::Polygon() = default;

Polygon::Polygon(std::vector<Point> points, Point minPoint, Point maxPoint) :
points(std::move(points)) , bbox(std::make_pair(minPoint, maxPoint)) {}

void Polygon::addPoint(const Point &point) {
	points.push_back(point);
}

void Polygon::setConvex() {
	convex = std::make_shared<sf::ConvexShape>();
	
	convex->setPointCount(points.size());
	convex->setFillColor(sf::Color(0, 0, 0, 200));
	
	int i = 0;
	for(const auto &point : points){
		convex->setPoint(i++, sf::Vector2f((float) point.x, (float) point.y));
	}
}

void Polygon::drawShape(sf::RenderWindow &window) const {

}
