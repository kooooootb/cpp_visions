#include "Point.h"
#include "Polygon.h"
#include "JSONDataNode.h"

Polygon::Polygon(const DataNode &dataNode) :
    VertexHolder(dataNode) , Bbox(dataNode.getPointsVector(VERTICESKEY)) , points(dataNode.getPointsVector(VERTICESKEY))
{}

const std::vector<Point> &Polygon::getPoints() const {
    return points;
}

std::string Polygon::serialize() const {
    using Formatter = JSONDataNode;
    return Formatter::formatPointsVector(VERTICESKEY, points);
}
