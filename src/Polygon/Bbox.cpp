#include <cassert>
#include <limits>
#include <algorithm>

#include "Bbox.h"

Bbox::Bbox(const std::vector<Point> &points) :
    bottomLeft(std::numeric_limits<double>::max(), std::numeric_limits<double>::max()),
    topRight(std::numeric_limits<double>::min(), std::numeric_limits<double>::min())
{
    for(const auto &point : points){
        bottomLeft.x = std::min(bottomLeft.x, point.x);
        bottomLeft.y = std::min(bottomLeft.y, point.y);
        topRight.x = std::max(topRight.x, point.x);
        topRight.y = std::max(topRight.y, point.y);
    }
}

bool Bbox::bboxColliding(const Point &bl, const Point &tr) const {
    return tr.x >= bottomLeft.x && tr.y >= bottomLeft.y && bl.x <= topRight.x && bl.y <= topRight.y;
}
