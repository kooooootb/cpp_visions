#include <SFML/Graphics.hpp>

#include <vector>
#include <list>
#include <cmath>
#include <memory>

#include "Common.h"

class Polygon{
private:
	std::vector<Point> points;
	bool visible = false;
	
	sf::VertexArray viewVertices;
    sf::Texture viewTexture = createPlainTexture(1, 1, sf::Color(2,2,2,255));

    std::pair<Point, Point> bbox;

    void setBBox();

    bool isInternalDiagonal(size_t p1, size_t p2, std::vector<size_t> &prevPoint, std::vector<size_t> &nextPoint);
    bool isDiagonal(size_t p1, size_t p2, std::vector<size_t> &nextPoint);
    bool isIntersecting(size_t p1, size_t p2, size_t p3, size_t p4);
    bool isProperlyIntersecting(size_t p1, size_t p2, size_t p3, size_t p4);
    bool isCollinear(size_t p1, size_t p2, size_t p3);
    double triangleArea(size_t p1, size_t p2, size_t p3);
    bool isBetween(size_t p1, size_t p2, size_t p3);
    bool inCone(size_t p1, size_t p2, std::vector<size_t> &prevPoint, std::vector<size_t> &nextPoint);
public:
    Polygon(std::vector<Point> points, Point minPoint, Point maxPoint);
    explicit Polygon(std::vector<Point> points);

    const std::vector<Point> &getPoints() const{ return points; }
    std::vector<Point> &getPoints(){ return points; }
    sf::VertexArray &getViewVertices(){ return viewVertices; }
    const sf::VertexArray &getViewVertices() const{ return viewVertices; }

	void addPoint(const Point &point);
    const Point &minPoint() const{ return bbox.first; }
    const Point &maxPoint() const{ return bbox.second; }

    void setViewShape();
	const sf::VertexArray &getViewShape()const { return viewVertices; }
    void draw(sf::RenderWindow &window)const;
};


