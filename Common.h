#ifndef T5_COMMON_H
#define T5_COMMON_H

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

    Point(float X, float Y) : x(X) , y(Y) {}

    Point() = default;

    float getDistance(const Point &point) const{
        return (float) sqrt(pow((x - point.x), 2) + pow((y - point.y), 2));
    }

    Point operator+(const Vector &vector) const;
    Point operator-(const Vector &vector) const;

    bool operator!=(const Point &point) const{
        return x != point.x || y != point.y;
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

const int screen_width = 800;
const int screen_height = 600;
const int point_radius = 2;

void drawAll(sf::RenderWindow &window, const std::vector<std::shared_ptr<sf::Shape>> &shapes);

void createBackground(std::vector<std::shared_ptr<sf::Shape>> &shapes);

void initViewSector(std::shared_ptr<sf::ConvexShape> &viewShape, float viewDistance, float viewAngle);

constexpr float degToRad(float degrees){ return (float) (degrees * M_PI / 180); }
constexpr float radToDeg(float radians){ return (float) (radians * 180 / M_PI); }

inline float getAngleToZero(const sf::Vector2f &a){
	if (a.y > 0)
		return (float) acos((a.x) / sqrt(pow(a.x, 2) + pow(a.y, 2)));
	else
		return (float) (2 * M_PI - acos((a.x) / sqrt(pow(a.x, 2) + pow(a.y, 2))));
}

#endif //T5_COMMON_H
