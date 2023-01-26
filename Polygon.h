#include <SFML/Graphics.hpp>

#include <vector>
#include <list>
#include <cmath>
#include <memory>

#include "Common.h"

//class PolygonPoint : public Point{
//public:
//    Polygon &polygon;
//    PointType type = outside;
//
//    PolygonPoint(Polygon &pol, float X, float Y) : polygon(pol) , Point(X, Y) {}
//};

class Polygon{
private:
	std::vector<Point> points;
	bool visible = false;
	
	std::shared_ptr<sf::ConvexShape> convex;

    std::pair<Point, Point> bbox;
public:
	Polygon();
	Polygon(std::vector<Point> points, Point minPoint, Point maxPoint);

    const std::vector<Point> &getPoints() const{ return points; }
	void addPoint(const Point &point);
    const Point &minPoint() const{ return bbox.first; }
    const Point &maxPoint() const{ return bbox.second; }
	void setConvex();

	const sf::ConvexShape &getConvex()const { return *convex; }
    void drawShape(sf::RenderWindow &window)const;
};


