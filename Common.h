#ifndef T5_COMMON_H
#define T5_COMMON_H

#include <memory>
#include <list>
#include <limits>
#include <cmath>

#include "headers.h"

using BoarderType = std::pair<unsigned int, unsigned int>;
using PolygonArray = std::vector<std::shared_ptr<Polygon>>;

enum PointType{ //ordering is important
    within = 0,
    behind = 1,
    semicircle = 2,
    outside = 3,
};

class Point{
public:
    double x, y;

    Point() : x(0) , y(0) {}
    Point(double X, double Y) : x(X) , y(Y) {}
    Point(int X, int Y) : x((double) X) , y((double) Y) {}
    explicit Point(const sf::Vector2i &vector) : x((double) vector.x) , y((double) vector.y) {}
    explicit Point(const sf::Vector2f &vector) : x((double) vector.x) , y((double) vector.y) {}

    double getDistance(const Point &point) const;
    double distSqr(const Point &point) const;
    double sqr() const;

    Point operator+(const Vector &vector) const;
    Point operator-(const Vector &vector) const;
    Point &operator+=(const Vector &vector);
    Point &operator+=(const sf::Vector2f &vector);
    friend Vector operator-(const Point &point, const sf::Vector2f &vector);
    friend Vector operator-(const Point &point, const sf::Vector2i &vector);

    bool operator!=(const Point &point) const;
    bool operator==(const sf::Vector2i &vector)const;
    bool operator==(const Point &point) const;

    sf::Vector2f toSfVector() const;
};

class Vector : public Point{
public:
    Vector(const Point &from, const Point &to) : Point(to.x - from.x, to.y - from.y) {}
    Vector(double X, double Y) : Point(X, Y) {}
    Vector(const Vector &vector) : Point(vector.x, vector.y) {}
    Vector() = default;

    double operator*(const Point &point) const;
    bool operator!=(const Vector &vector) const;
    double cross(const Point &point) const;
    double cross(const Vector &vector) const;
    Vector operator/(double k) const;
    friend Vector operator*(const Vector &v, double k);
    friend Vector operator*(double k, const Vector &v);
    Vector perpendicular() const;

    double sqr()const;
    double length()const;
    void increase(double k);
    void move(double dx, double dy);
    void rotate(double radians);
};


int initViewSector(sf::VertexArray &viewShape, const Point &center, double viewDistance, double currentAngle, double viewAngle);

// angle funcs
constexpr double degToRad(double degrees) { return (degrees * M_PI / 180); }
constexpr double radToDeg(double radians) { return (radians * 180 / M_PI); }
double getAngleToZero(const Vector &a);

// geometry funcs
bool isOutOfBoarders(const Vector &pointVector, const std::array<Vector, 3> &views);
Point rayCheck(const Point &center, double radius, const Vector &pointVector, const std::vector<Edge> &blockingEdges);
Point calcClosestPoint(const Edge &edge, const Point &point);
bool findClosestEdge(const std::vector<Edge> &edges, Edge &edge, const Point &point);

// loading saving funcs
void skWp(std::ifstream &fd);
void skBd(std::ifstream &fd);
void svLv(const std::string &fname, const std::vector<std::shared_ptr<Polygon>> &polygons, const std::vector<Point> &weaponSps, const BoarderType &boarders);
std::vector<std::shared_ptr<Polygon>> ldPl(const std::string &fname);
std::vector<Point> ldWp(const std::string &fname);
BoarderType ldBd(const std::string &fname);

Point findNearestPoint(const Point &center, const std::vector<Point> &points);

// intersection funcs
std::vector<Point> arcSegmentVSLineIntersection(const Point &p1, const Point &p2, const Point &center, double radius, const std::array<Vector, 3> &views);
std::vector<Point> circleVSLineIntersection(const Point &p1, const Point &p2, const Point &center, float radius);
bool circleVSLineIntersectionCheck(const Point &p1, const Point &p2, const Point &center, double radius);
std::vector<Point> twoLinesVSLineIntersection(const Point &p1, const Point &p2, const Point &center, const std::array<Vector, 3> &views);
bool unlimitedLineVSLineIntersection(const Point &p1, const Vector &d0, const Point &p3, const Vector &d1, Point &res);
bool lineVSLineIntersection(const Point &p1, const Vector &d0, const Point &p3, const Vector &d1, Point &res);
bool lineVSLineIntersectionCheck(const Point &p1, const Vector &d0, const Point &p3, const Vector &d1);
bool vectorVSEdgesIntersectionCheck(const Point &center, const Vector &vector, const std::vector<Edge> &edges);
std::vector<Point> vectorVSEdgesIntersection(const Point &center, const Vector &vector, const std::vector<Edge> &edges);

void writeFname(std::string &fname, std::ofstream &fd);
void writeFname(std::ofstream &fd);
std::string readFname(std::ifstream &fd);

sf::Vector2f getMousePosition(sf::RenderWindow &window, const sf::View &view);

void createBackground(sf::VertexArray &bgVertices, sf::Texture &bgTexture, unsigned int width, unsigned int height);
sf::Texture createPlainTexture(unsigned int width, unsigned int height, const sf::Color &color = sf::Color::Black);

bool almost_equal(double x, double y, int ulp=2);
bool almost_equal(const Point &p1, const Point &p2);
bool almost_equal(const Edge &e1, const Edge &e2);

std::string getStringFromWindow(sf::RenderWindow &window, const std::string &message);
void addBoarders(PolygonArray &polygons, BoarderType &boarders); // add 4 rectangles to polygons
std::vector<Point> vertexArrayToPoints(sf::VertexArray &v);
sf::VertexArray createGridVertices(BoarderType::first_type width, BoarderType::second_type height, float precision);
sf::CircleShape setAimPointShape(float radius = 6);
void applyFriction(sf::Vector2f &vector, float friction);
bool checkKeyPressed(std::initializer_list<sf::Keyboard::Key>);

void setVerticesAsBox(sf::VertexArray &vertices, const sf::Vector2f &center, const sf::Vector2f &vector);

bool checkFileName(const std::string &string);

#endif //T5_COMMON_H




















