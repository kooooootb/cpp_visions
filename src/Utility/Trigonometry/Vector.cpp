#include <cmath>
#include <limits>
#include <algorithm>

#include "Vector.h"
#include "Utility.h"

Vector::Vector(const Point &from, const Point &to) : Point(to.x - from.x, to.y - from.y) {}

Vector::Vector(double X, double Y) : Point(X, Y) {}

Vector::Vector(const Vector &vector) : Point(vector.x, vector.y) {}

double Vector::operator*(const Point &point) const {
    return (x * point.x + y * point.y);
}

bool Vector::operator!=(const Vector &vector) const {
    return static_cast<const Point &>(*this) != static_cast<const Point &>(vector);
}

double Vector::cross(const Point &point) const {
    return (x * point.y - y * point.x);
}

double Vector::cross(const Vector &vector) const {
    return (x * vector.y - y * vector.x);
}

Vector Vector::operator/(double k) const {
    return { x / k, y / k };
}

Vector operator*(const Vector &v, double k) {
    return { v.x * k, v.y * k };
}

Vector operator*(double k, const Vector &v) {
    return { v.x * k, v.y * k };
}

Vector Vector::perpendicular() const {
    return { y, -x };
}

double Vector::length() const { return std::sqrt(sqr()); }

void Vector::increase(double k) { x *= k; y *= k; }

void Vector::move(double dx, double dy) { x += dx; y += dy; }

void Vector::rotate(double radians) {
    double cs = std::cos(radians);
    double sn = std::sin(radians);

    double newX = x * cs - y * sn;
    y = x * sn + y * cs;
    x = newX;
}

Vector &Vector::operator/=(double k) {
    x /= k;
    y /= k;
    return *this;
}

Vector &Vector::operator*=(double k) {
    x *= k;
    y *= k;
    return *this;
}

bool Vector::lengthEqual(double length) const {
    double vector_2 = sqr();
    double length_2 = std::pow(length, 2);
    return Util::almostEqual(vector_2, length_2);
}

bool Vector::angleEqual(const Vector &vector) const {
//    double l1_2 = sqr();
//    double l2_2 = vector.sqr();
//
//    double averageLen_2 = (l1_2 + l2_2) / 2;
//
//    double cosValue = (*this * vector) / averageLen_2; // approximately
//    return Util::almostEqual(cosValue, 1.);

    return Util::almostEqual(std::abs(cross(vector)), 0.);
}
