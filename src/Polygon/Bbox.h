#ifndef VISIONS_BBOX_H
#define VISIONS_BBOX_H

#include "Point.h"

class Bbox {
protected:
    Point bottomLeft, topRight;
public:
    explicit Bbox(const std::vector<Point> &points);

    virtual ~Bbox() = default;

    bool bboxColliding(const Point &bl, const Point &tr) const;
};


#endif //VISIONS_BBOX_H
