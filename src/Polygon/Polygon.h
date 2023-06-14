#ifndef VISIONS_POLYGONS_H
#define VISIONS_POLYGONS_H

#include <SFML/Graphics.hpp>

#include <vector>
#include <list>
#include <cmath>
#include <memory>

#include "VertexHolder.h"
#include "Bbox.h"
#include "DataNode.h"

class Polygon : public Bbox , public VertexHolder {
private:
    std::vector<Point> points;

    static constexpr char VERTICESKEY[] = "vertices"; // same as in VertexHolder!

public:
    explicit Polygon(const DataNode &dataNode);

    ~Polygon() override = default;

    /**
     * unify array type
     */
    using Array = std::shared_ptr<std::vector<std::unique_ptr<Polygon>>>;

    const std::vector<Point> &getPoints() const;

    std::string serialize() const;
};

#endif //VISIONS_POLYGONS_H