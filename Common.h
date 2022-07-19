#ifndef T5_COMMON_H
#define T5_COMMON_H

#include <memory>
#include <list>
#include <limits>
#include <cmath>

#include "headers.h"

enum PointType{ //ordering is important
    within = 0,
    behind = 1,
    semicircle = 2,
    outside = 3,
};

class Point{
public:
    float x, y;

    Point() = default;
    Point(float X, float Y) : x(X) , y(Y) {}
    Point(const sf::Vector2i &vector) : x((float) vector.x) , y((float) vector.y) {}

    float getDistance(const Point &point) const{
        return (float) sqrt(pow((x - point.x), 2) + pow((y - point.y), 2));
    }

    Point operator+(const Vector &vector) const;
    Point operator-(const Vector &vector) const;
    Point &operator+=(const Vector &vector);
    Point &operator+=(const sf::Vector2f &vector);
    friend Vector operator-(const Point &point, const sf::Vector2f &vector);
    friend Vector operator-(const Point &point, const sf::Vector2i &vector);

    bool operator!=(const Point &point) const{
        return x != point.x || y != point.y;
    }

    bool operator==(const sf::Vector2i &vector)const{
        return x == vector.x && y == vector.y;
    }

    bool operator==(const Point &point) const{
        return x == point.x && y == point.y;
    }
};

class Vector : public Point{
public:
    Vector(const Point &from, const Point &to) : Point(to.x - from.x, to.y - from.y) {}
    Vector(float X, float Y) : Point(X, Y) {}
    Vector(const Vector &vector) : Point(vector.x, vector.y) {}
    Vector() = default;

    float operator*(const Point &point) const{
        return (x * point.x + y * point.y);
    }

    bool operator!=(const Vector &vector) const{
        return x != vector.x || y != vector.y;
    }

    float cross(const Point &point) const{
        return (x * point.y - y * point.x);
    }

    Vector operator/(float k) const{
        return { x / k, y / k };
    }

    friend Vector operator*(const Vector &v, float k){
        return { v.x * k, v.y * k };
    }

    friend Vector operator*(float k, const Vector &v){
        return { v.x * k, v.y * k };
    }

    Vector perpendicular() const {
        return { y, -x };
    }

    float sqr()const { return (x * x + y * y); }
    float length()const { return sqrt(x * x + y * y); }
    void increase(float k) { x *= k; y *= k; }
    void move(float dx, float dy) { x += dx; y += dy; }
    void rotate(float radians) {
        float cs = cos(radians);
        float sn = sin(radians);

        float newX = x * cs - y * sn;
        y = x * sn + y * cs;
        x = newX;
    }
};


int initViewSector(std::shared_ptr<sf::ConvexShape> &viewShape, const Point &center, float viewDistance, float currentAngle, float viewAngle);

constexpr float degToRad(float degrees){ return (float) (degrees * M_PI / 180); }
constexpr float radToDeg(float radians){ return (float) (radians * 180 / M_PI); }

inline float getAngleToZero(const sf::Vector2f &a){
    if(a.x == 0 && a.y == 0){
        return 0.0;
    }else if(a.x > 0){
        return atan(a.y / a.x);
    }else{
        return atan(a.y / a.x) + M_PI;
    }
}

bool isOutOfBoarders(const Vector &pointVector, const std::array<Vector, 3> &views);

Point calcClosestPoint(const Edge &edge, const Point &point);
bool findClosestEdge(const std::vector<Edge> &edges, Edge &edge, const Point &point);

void skipWeaponSpawnpoints(std::ifstream &fd);
void skipBoarders(std::ifstream &fd);
void saveLevel(const std::string &fname, const std::vector<std::shared_ptr<std::list<sf::Vector2f>>> &polygons,
               const std::vector<Point> &weaponSps, const std::pair<int, int> &boarders);
bool loadLevel(std::vector<std::shared_ptr<sf::Shape>> &shapes, const std::string &fname,
               std::vector<std::shared_ptr<std::list<sf::Vector2f>>> &polygons, std::vector<Point> &weaponSps);
Polygons loadLevelForTree(const std::string &fname);
std::vector<Point> loadWeaponSpawnpoints(std::ifstream &fd);
std::vector<Point> loadWeaponSpawnpoints(const std::string &fname);
std::pair<int, int> loadBoarders(const std::string &fname);

Point findNearestPoint(const Point &center, const std::vector<Point> &points);

std::vector<Point> arcSegmentVSLineIntersection(const Point &p1, const Point &p2, const Point &center, float radius, const std::array<Vector, 3> &views);
std::vector<Point> circleVSLineIntersection(const Point &p1, const Point &p2, const Point &center, float radius);
bool circleVSLineIntersectionCheck(const Point &p1, const Point &p2, const Point &center, float radius);
std::vector<Point> twoLinesVSLineIntersection(const Point &p1, const Point &p2, const Point &center, const std::array<Vector, 3> &views);
bool unlimitedLineVSLineIntersection(const Point &p1, const Vector &d0, const Point &p3, const Vector &d1, Point &res);
bool lineVSLineIntersection(const Point &p1, const Vector &d0, const Point &p3, const Vector &d1, Point &res);
bool lineVSLineIntersectionCheck(const Point &p1, const Vector &d0, const Point &p3, const Vector &d1);
bool vectorVSEdgesIntersectionCheck(const Point &center, const Vector &vector, const std::vector<Edge> &edges);
std::vector<Point> vectorVSEdgesIntersection(const Point &center, const Vector &vector, const std::vector<Edge> &edges);

void writeFname(std::string &fname, std::ofstream &fd);
void writeFname(std::ofstream &fd);
std::string readFname(std::ifstream &fd);

sf::Vector2f getMousePosition(const sf::RenderWindow &window);

#endif //T5_COMMON_H
