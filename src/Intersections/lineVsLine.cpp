#include "Intersections.h"

namespace Inter {
    std::unique_ptr<Point> infLineVSLine(const Point &p1, const Vector &d0, const Point &p3, const Vector &d1) {
        const Vector delta(p1, p3);
        double denominator = d0 * (d1.perpendicular());

        if (denominator != 0) {
            double t = (delta * (d0.perpendicular())) / denominator;
            double s = (delta * (d1.perpendicular())) / denominator;
            if (t >= 0 && t <= 1 && s >= 0) {
                return std::make_unique<Point>(p3 + (t * d1));
            }
        }

        return nullptr;
    }

    bool infLineVSLineCheck(const Point &p1, const Vector &d0, const Point &p3, const Vector &d1) {
        const Vector delta(p1, p3);
        double denominator = d0 * (d1.perpendicular());

        if (denominator != 0) {
            double t = (delta * (d0.perpendicular())) / denominator;
            double s = (delta * (d1.perpendicular())) / denominator;
            if (t >= 0 && t <= 1 && s >= 0) {
                return true;
            }
        }

        return false;
    }

    std::unique_ptr<Point> lineVSLine(const Point &p1, const Vector &d0, const Point &p3, const Vector &d1) {
        const Vector delta(p1, p3);
        double denominator = d0 * (d1.perpendicular());

        if (denominator != 0) {
            double t = (delta * (d0.perpendicular())) / denominator;
            double s = (delta * (d1.perpendicular())) / denominator;
            if (t >= 0 && t <= 1 && s >= 0 && s <= 1) {
                return std::make_unique<Point>(p3 + (t * d1));
            }
        }

        return nullptr;
    }

    bool lineVSLineCheck(const Point &p1, const Vector &d0, const Point &p3, const Vector &d1) {
        const Vector delta(p1, p3);
        double denominator = d0 * (d1.perpendicular());

        if (denominator != 0) {
            double t = (delta * (d0.perpendicular())) / denominator;
            double s = (delta * (d1.perpendicular())) / denominator;
            if (t >= 0 && t <= 1 && s >= 0 && s <= 1) {
                return true;
            }
        }

        return false;
    }

    bool lineVSLineCheck(const Edge &edge1, const Edge &edge2) {
        return lineVSLineCheck(edge1.first, Vector(edge1.first, edge1.second), edge2.first, Vector(edge2.first, edge2.second));
    }

    std::unique_ptr<Point> lineVSLine(const Edge &edge1, const Edge &edge2) {
        return lineVSLine(edge1.first, Vector(edge1.first, edge1.second), edge2.first, Vector(edge2.first, edge2.second));
    }
}