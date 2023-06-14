#include <cmath>

#include "Point.h"

Point::Point(double X, double Y) : x(X) , y(Y) {}

Point::Point(const sf::Vector2i &vector) : x((double) vector.x) , y((double) vector.y) {}

Point::Point(const sf::Vector2f &vector) : x((double) vector.x) , y((double) vector.y) {}

Point Point::operator+(const Point &vector) const {
    return { x + vector.x, y + vector.y };
}

Point Point::operator-(const Point &vector) const {
    return { x - vector.x, y - vector.y };
}

Point &Point::operator+=(const Point &vector) {
    x += vector.x;
    y += vector.y;

    return *this;
}

Point &Point::operator+=(const sf::Vector2f &vector) {
    x += (double) vector.x;
    y += (double) vector.y;

    return *this;
}

Point operator-(const Point &point, const sf::Vector2f &vector) {
    return { point.x - vector.x, point.y - vector.y };
}

Point operator-(const Point &point, const sf::Vector2i &vector) {
    return { point.x - vector.x, point.y - vector.y };
}

double Point::distSqr(const Point &point) const {
    return std::pow((x - point.x), 2) + std::pow((y - point.y), 2);
}

double Point::getDistance(const Point &point) const {
    return (double) std::sqrt(distSqr(point));
}

bool Point::operator!=(const Point &point) const {
    double eps = std::numeric_limits<double>::epsilon();
    double min = std::numeric_limits<double>::min();
    double diff = std::abs(x - point.x) + std::abs(y - point.y);

    return diff < min || diff < eps * 2 * (std::abs(x - point.x) + std::abs(y - point.y));
}

bool Point::operator==(const sf::Vector2i &vector) const {
    return x == vector.x && y == vector.y;
}

bool Point::operator==(const Point &point) const {
    return x == point.x && y == point.y;
}

double Point::sqr() const {
    return x * x + y * y;
}

sf::Vector2f Point::toSfVector() const {
    return {(float) x, (float) y};
}
