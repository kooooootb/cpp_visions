#ifndef VISIONS_POINT_H
#define VISIONS_POINT_H

#include <SFML/Graphics.hpp>

class Point{
public:
    double x, y;

    Point(double X, double Y);
    explicit Point(const sf::Vector2i &vector);
    explicit Point(const sf::Vector2f &vector);

    double getDistance(const Point &point) const;
    double distSqr(const Point &point) const;
    double sqr() const;

    Point operator+(const Point &vector) const;
    Point operator-(const Point &vector) const;
    Point &operator+=(const Point &vector);
    Point &operator+=(const sf::Vector2f &vector);
    friend Point operator-(const Point &point, const sf::Vector2f &vector);
    friend Point operator-(const Point &point, const sf::Vector2i &vector);

    bool operator!=(const Point &point) const;
    bool operator==(const sf::Vector2i &vector)const;
    bool operator==(const Point &point) const;

    sf::Vector2f toSfVector() const;
};


#endif //VISIONS_POINT_H
