#ifndef VISIONS_VISIONING_H
#define VISIONS_VISIONING_H

#include <SFML/Graphics.hpp>

#include <memory>
#include <queue>

#include "Entity.h"
#include "EntityStorage.h"
#include "Polygon.h"
#include "DataNode.h"
#include "Edge.h"
#include "Vector.h"

class Visioning : virtual public Entity {
private:
    using Views_t = std::array<Vector, 3>;
    using PointsList_t = std::list<Point>;
    using PointsVector_t = std::vector<Point>;
    using EdgesArray_t = std::list<Edge>;

    // what we see
    std::unique_ptr<sf::VertexArray> viewShape;

    // vectors of our vision
    Views_t views;

    double fieldOfView;
    double viewLength; // more like view distance

    // collecting points and edges
    PointsList_t arcPoints, endPoints;
    EdgesArray_t blockingEdges;

    static constexpr char FOVKEY[] = "fov";
    static constexpr char VIEWLENGTHKEY[] = "view length";

    enum PointType{ //ordering is important, specifying where point is relative to player
        within = 0, // within view sector
        behind = 1,
        semicircle = 2,
        outside = 3,
    };

    void updateViewVectors();
    PointsVector_t collectViewPoints(const Polygon::Array &polygons);
    void processPolygon(const Polygon &polygon);
    PointType getPointType(const Point &point);
    PointsVector_t toRespoints(PointsVector_t &hitPoints) const;
    void setViewShape(PointsVector_t &resPoints);

    Point rayCheck(const Vector &pointVector);
    std::unique_ptr<Edge> findClosestBlockingEdge(const Point &point);
    void addViewSector(double currentAngle, double viewAngle);
    void addEdgeSector(const Point &p1, const Point &p2);

    static Point calcClosestPoint(const Edge &edge, const Point &point);
    static bool inSemicircle(const Views_t &views, const Vector &vector);
    static void setVertex(sf::Vertex &vertex, const sf::Vector2f &vector); // set single vertex by point

protected:
    void setAngle(double angle_) override;

public:
    explicit Visioning(const DataNode &dataNode);

    Visioning(const Visioning &vis);

    ~Visioning() override = default;

    void updateViewShape(const Polygon::Array &polygons);
    void updateSeeingEntities(const EntityStorage &entityStorage);

    void drawViewShape(WindowHolder &window, const sf::Texture &texture) const;

    std::string serialize() const;
};


#endif //VISIONS_VISIONING_H
