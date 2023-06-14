#ifndef VISIONS_VERTEXHOLDER_H
#define VISIONS_VERTEXHOLDER_H

#include <SFML/Graphics.hpp>

#include <memory>

#include "Drawable.h"
#include "DataNode.h"
#include "WindowHolder.h"

class VertexHolder : public Drawable {
private:
    std::shared_ptr<sf::VertexArray> vertices;
    std::shared_ptr<sf::Texture> texture;

    static constexpr char TEXTUREKEY[] = "texture";
    static constexpr char VERTICESKEY[] = "vertices"; // same as in Polygon!

    void loadTexture(const std::filesystem::path &folder);

    // configure viewing vertices
    void setVertices(const std::vector<Point> &points);
    static void setVertex(sf::Vertex &vertex, const Point &point); // set single vertex by point

    // for triangulation
    static bool isInternalDiagonal(const std::vector<Point>& points, size_t p1, size_t p2, std::vector<size_t> &prevPoint, std::vector<size_t> &nextPoint);
    static bool isDiagonal(const std::vector<Point>& points, size_t p1, size_t p2, std::vector<size_t> &nextPoint);
    static bool isIntersecting(const std::vector<Point> &points, size_t p1, size_t p2, size_t p3, size_t p4);
    static bool isProperlyIntersecting(const std::vector<Point> &points, size_t p1, size_t p2, size_t p3, size_t p4);
    static bool isCollinear(const std::vector<Point>& points, size_t p1, size_t p2, size_t p3);
    static double triangleArea(const std::vector<Point>& points, size_t p1, size_t p2, size_t p3);
    static bool isBetween(const std::vector<Point>& points, size_t p1, size_t p2, size_t p3);
    static bool inCone(const std::vector<Point>& points, size_t p1, size_t p2, std::vector<size_t> &prevPoint, std::vector<size_t> &nextPoint);
public:
    explicit VertexHolder(const DataNode &dataNode);
    VertexHolder(const VertexHolder &vh);

    ~VertexHolder() override = default;

    void draw(WindowHolder &window) const override;

    [[nodiscard]] std::shared_ptr<sf::Texture> getTexture() const;
    void setColor(sf::Color color);

    // this class will not save points as they are triangulated
};


#endif //VISIONS_VERTEXHOLDER_H
