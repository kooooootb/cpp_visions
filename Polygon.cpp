#include "Polygon.h"
#include <iostream>

Polygon::Polygon() = default;

void Polygon::addBPoint(const Point &point) {
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

void Polygon::connectBPoints(const Point &bp1, const Point &bp2) {
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

void Polygon::updateVisibility() {
	convex->setFillColor(sf::Color(0, 0, 0, visible ? 200 : 100));
}

void Polygons::add(Polygon &polygon) {
	polygons.push_back(std::move(polygon));
	
	for(const auto &it : polygons.back().boarderPoints){
		points.push_back(it);
	}
	
	for(const auto &it : polygons.back().extraPoints){
		points.push_back(it);
	}
}

std::vector<std::shared_ptr<sf::Shape>> Polygons::collectShapes() const {
	std::vector<std::shared_ptr<sf::Shape>> shapes;
	
	for(const auto &it : polygons){
		shapes.push_back(it.convex);
	}
	
	return shapes;
}

void Polygons::hideAll() {
	for(auto &it : polygons){
		it.makeInvisible();
	}
}

Point &Polygons::operator[](unsigned int index) {
	return points[index];
}
