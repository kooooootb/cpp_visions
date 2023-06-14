#ifndef VISIONS_INTERSECTIONS_H
#define VISIONS_INTERSECTIONS_H

#include <vector>
#include <memory>
#include <cmath>

#include "Point.h"
#include "Vector.h"
#include "Edge.h"

namespace Inter{
    // line to line
    std::unique_ptr<Point> infLineVSLine(const Point &p1, const Vector &d0, const Point &p3, const Vector &d1);
    bool infLineVSLineCheck(const Point &p1, const Vector &d0, const Point &p3, const Vector &d1);
    std::unique_ptr<Point> lineVSLine(const Point &p1, const Vector &d0, const Point &p3, const Vector &d1);
    std::unique_ptr<Point> lineVSLine(const Edge &edge1, const Edge &edge2);
    bool lineVSLineCheck(const Point &p1, const Vector &d0, const Point &p3, const Vector &d1);
    bool lineVSLineCheck(const Edge &edge1, const Edge &edge2);

    // many lines to line(s)
    template<typename I>
    I edgeVSEdges(const Edge &edge, const I &edges);
    template<typename I>
    bool edgeVSEdgesCheck(const Edge &edge, const I &edges);
    template<typename I>
    I lineVSBrokenLine(const Point &p1, const Point &p2, const Point &center, const Vector &v1, const Vector &v2);
    template<typename I>
    I lineVSBrokenLine(const Edge &edge, const Point &center, const Vector &v1, const Vector &v2);

    // circles and arcs
    bool circleVSLineCheck(const Point &p1, const Point &p2, const Point &center, double radius);
    std::vector<Point> circleVSLine(const Point &p1, const Point &p2, const Point &center, float radius);
    template<typename I>
    std::vector<Point> arcVSLine(const Point &p1, const Point &p2, const Point &center, double radius, const I &vectors);

    template<typename I>
    I edgeVSEdges(const Edge &edge, const I &edges) {
        I result;
        std::unique_ptr<Point> intersection = nullptr;

        for (const auto &edge_ : edges) {
            intersection = lineVSLine(edge, edge_);
            if (intersection != nullptr) {
                result.push_back(*intersection);
            }
        }

        return result;
    }

    template<typename I>
    bool edgeVSEdgesCheck(const Edge &edge, const I &edges) {
        for (const auto &edge_: edges) {
            if (lineVSLineCheck(edge, edge_)) {
                return true;
            }
        }

        return false;
    }

    template<typename I>
    I lineVSBrokenLine(const Edge &edge, const Point &center, const Vector &v1, const Vector &v2) {
        return lineVSBrokenLine<I>(edge.first, edge.second, center, v1, v2);
    }

    template<typename I>
    I lineVSBrokenLine(const Point &p1, const Point &p2, const Point &center, const Vector &v1, const Vector &v2) {
        //1 line: p1 + s * d0
        const Vector d0(p1, p2);
        //2 line: center + t * views[0]
        //3 line: center + t * views[2]
        //p1 == P0 ; center == P1

        const Vector delta(p1, center);

        I res;
        const Vector *vectors[2] = {&v1, &v2};

        for(const auto &d1 : vectors){
            double denominator = d0 * (d1->perpendicular());

            if(denominator != 0){
                double t = (delta * (d0.perpendicular())) / denominator;
                double s = (delta * (d1->perpendicular())) / denominator;
                if(t >= 0 && t <= 1 && s >= 0 && s <= 1){
                    res.push_back(center + (t * *d1));
                }
            }
        }

        return res;
    }

    template<typename I>
    std::vector<Point>
    arcVSLine(const Point &p1, const Point &p2, const Point &center, double radius, const I &vectors) {
        //|X^2 - center^2| = radius^2
        //X(t) = p1 + td , (t from 0 to 1)
        //delta = p1 - center
        Vector d(p1, p2);
        Vector delta(center, p1);

        std::vector<Point> res;
        res.reserve(2);

        auto isInSight = [&](const Point &point) -> bool {
            Vector pV(center, point);
            if(pV * vectors[1] < 0){
                return false;
            }else if(vectors[0].cross(pV) * pV.cross(vectors[2]) > 0){
                return true;
            }else{
                return false;
            }
        };

        double discriminant = (d * delta) * (d * delta) - d.sqr() * (delta.sqr() - radius * radius);

        if(discriminant < 0){
            return res;
        }else if(discriminant == 0){
            double t = (-1 * (d * delta)) / (d.sqr());
            if(t >= 0 && t <= 1){
                Point p(p1 + t * d);
                if(isInSight(p)){
                    res.push_back(p);
                }
            }
            return res;
        }else{
            discriminant = std::sqrt(discriminant);

            double t;
            t = ((-1 * (d * delta)) + discriminant) / (d.sqr());
            if(t >= 0 && t <= 1){
                Point p(p1 + t * d);
                if(isInSight(p)){
                    res.push_back(p);
                }
            }
            t = ((-1 * (d * delta)) - discriminant) / (d.sqr());
            if(t >= 0 && t <= 1){
                Point p(p1 + t * d);
                if(isInSight(p)){
                    res.push_back(p);
                }
            }

            return res;
        }
    }
}

#endif //VISIONS_INTERSECTIONS_H
