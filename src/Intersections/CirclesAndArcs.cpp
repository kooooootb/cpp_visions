#include <cmath>

#include "Intersections.h"

namespace Inter{
    std::vector<Point> circleVSLine(const Point &p1, const Point &p2, const Point &center, double radius){//vector is limited (p2)
        //|X^2 - center^2| = radius^2
        //X(t) = p1 + td , (t from 0 to 1)
        //delta = p1 - center
        Vector d(p1, p2);
        Vector delta(center, p1);

        std::vector<Point> res;

        double discriminant = (d * delta) * (d * delta) - d.sqr() * (delta.sqr() - radius * radius);

        if(discriminant < 0){
            return res;
        }else if(discriminant == 0){
            double t = (-1 * (d * delta)) / (d.sqr());
            if(t >= 0){
                Point p(p1 + t * d);
                res.push_back(p);
            }
            return res;
        }else{
            discriminant = std::sqrt(discriminant);

            double t;
            t = ((-1 * (d * delta)) + discriminant) / (d.sqr());
            if(t >= 0 && t <= 1){
                Point p(p1 + t * d);
                res.push_back(p);
            }
            t = ((-1 * (d * delta)) - discriminant) / (d.sqr());
            if(t >= 0 && t <= 1){
                Point p(p1 + t * d);
                res.push_back(p);
            }

            return res;
        }
    }

    bool circleVSLineCheck(const Point &p1, const Point &p2, const Point &center, double radius){//check if line segment intersects circle 2 times
        //if line segment can intersect circle only 1 time check if 1 of its points inside circle
        Vector d(p1, p2);
        Vector delta(center, p1);

        std::vector<Point> res;

        double discriminant = (d * delta) * (d * delta) - d.sqr() * (delta.sqr() - radius * radius);

        if(discriminant < 0){
            return false;
        }else{
            double t = (-1 * (d * delta)) / (d.sqr());
            if(t >= 0 && t <= 1){
                return true;
            }
            else{
                return false;
            }
        }
    }
}
