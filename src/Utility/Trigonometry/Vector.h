#ifndef VISIONS_VECTOR_H
#define VISIONS_VECTOR_H

#include "Point.h"

class Vector : public Point{
public:
    Vector(const Point &from, const Point &to);
    Vector(double X, double Y);
    Vector(const Vector &vector);
    Vector() = default;

    double operator*(const Point &point) const;
    bool operator!=(const Vector &vector) const;
    double cross(const Point &point) const;
    double cross(const Vector &vector) const;
    Vector operator/(double k) const;
    Vector &operator/=(double k);
    Vector &operator*=(double k);
    friend Vector operator*(const Vector &v, double k);
    friend Vector operator*(double k, const Vector &v);
    Vector perpendicular() const;

    double length()const;
    void increase(double k);
    void move(double dx, double dy);
    void rotate(double radians);
    bool lengthEqual(double length) const;
    bool angleEqual(const Vector &vector) const;
};


#endif //VISIONS_VECTOR_H
