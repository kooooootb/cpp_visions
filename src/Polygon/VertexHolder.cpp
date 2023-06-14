#include <cassert>

#include "VertexHolder.h"
#include "Vector.h"

VertexHolder::VertexHolder(const DataNode &dataNode)  {
    loadTexture(dataNode.getFolder() / dataNode.getString(TEXTUREKEY));
    vertices = std::make_shared<sf::VertexArray>(sf::Triangles);
    setVertices(dataNode.getPointsVector(VERTICESKEY));
}

VertexHolder::VertexHolder(const VertexHolder &vh) {
    texture = vh.texture;
    vertices = vh.vertices;
}

void VertexHolder::loadTexture(const std::filesystem::path &file) {
    texture = std::make_unique<sf::Texture>();
    bool loaded = texture->loadFromFile((file).string());
    assert(loaded && "Can't load TEXTUREFILE in VertexHolder");
    texture->setRepeated(true);
}

void VertexHolder::setVertices(const std::vector<Point> &points) {
    size_t n = points.size();
    std::vector<size_t> prevPoint(points.size());
    std::vector<size_t> nextPoint(points.size());
    prevPoint.front() = n - 1;
    prevPoint.back() = n - 2;
    nextPoint.front() = 1;
    nextPoint.back() = 0;

    for(size_t i = 1;i < n - 1;++i){
        prevPoint[i] = i - 1;
        nextPoint[i] = i + 1;
    }

    std::vector<bool> isEars(n);
    for(size_t i = 0;i < n;++i){
        isEars[i] = isInternalDiagonal(points, prevPoint[i], nextPoint[i], prevPoint, nextPoint);
    }

    int triangleNum = 0;
    size_t i0, i1, i2, i3, i4;
    i2 = 0;
    size_t prevWorked = i2;
    bool didTwice = false;
    vertices->resize(3 * (n - 2));
    while(triangleNum < n - 3){
        if(isEars[i2]){
            i1 = prevPoint[i2];
            i0 = prevPoint[i1];
            i3 = nextPoint[i2];
            i4 = nextPoint[i3];

            prevPoint[i3] = i1;
            nextPoint[i1] = i3;

            isEars[i1] = isInternalDiagonal(points, i0, i3, prevPoint, nextPoint);
            isEars[i2] = false;
            isEars[i3] = isInternalDiagonal(points, i1, i4, prevPoint, nextPoint);

            setVertex((*vertices)[triangleNum * 3 + 0], points[i3]);
            setVertex((*vertices)[triangleNum * 3 + 1], points[i1]);
            setVertex((*vertices)[triangleNum * 3 + 2], points[i2]);
            triangleNum += 1;

            prevWorked = nextPoint[i2];
            didTwice = false;
        } else{ // TODO
            if(i2 == prevWorked && std::all_of(std::cbegin(isEars), std::cend(isEars), [](bool el) -> bool {
                return !el;
            })){
                if(didTwice){
                    prevPoint[nextPoint[i2]] = nextPoint[i2];
                    nextPoint[prevPoint[i2]] = prevPoint[i2];
                    triangleNum += 1;
                } else {
                    std::swap(nextPoint, prevPoint);
                    for (size_t i = 0; i < n; ++i) {
                        isEars[i] = isInternalDiagonal(points, prevPoint[i], nextPoint[i], prevPoint, nextPoint);
                    }
                    didTwice = true;
                }
            }
        }

        i2 = nextPoint[i2];
    }

    // Add remaining vertices
    setVertex((*vertices)[triangleNum * 3 + 0], points[i2]);
    setVertex((*vertices)[triangleNum * 3 + 1], points[nextPoint[i2]]);
    setVertex((*vertices)[triangleNum * 3 + 2], points[prevPoint[i2]]);
}

bool VertexHolder::isInternalDiagonal(const std::vector<Point>& points, size_t p1, size_t p2, std::vector<size_t> &prevPoint, std::vector<size_t> &nextPoint) {
    return inCone(points, p1, p2, prevPoint, nextPoint) && inCone(points, p2, p1, prevPoint, nextPoint) && isDiagonal(points, p1, p2, nextPoint);
}

bool VertexHolder::isDiagonal(const std::vector<Point> &points, size_t p1, size_t p2, std::vector<size_t> &nextPoint) {
    size_t p = p1, pn = nextPoint[p];

    do{
        if(p != p1 && p != p2 && pn != p1 && pn != p2){
            if(isIntersecting(points, p1, p2, p, pn)){
                return false;
            }
        }

        p = pn;
        pn = nextPoint[pn];
    }while(p != p1);

    return true;
}

bool VertexHolder::isIntersecting(const std::vector<Point>& points, size_t p1, size_t p2, size_t p3, size_t p4){
    if(isProperlyIntersecting(points, p1, p2, p3, p4)){
        return true;
    } else if(isBetween(points, p1, p2, p3) || isBetween(points, p1, p2, p4) || isBetween(points, p3, p4, p1) || isBetween(points, p3, p4, p2)) {
        return true;
    } else{
        return false;
    }
}

bool VertexHolder::isProperlyIntersecting(const std::vector<Point> &points, size_t p1, size_t p2, size_t p3, size_t p4){
    if(isCollinear(points, p1, p2, p3) || isCollinear(points, p1, p2, p4) || isCollinear(points, p3, p4, p1) || isCollinear(points, p3, p4, p2)){
        return false;
    } else{
        bool t1 = triangleArea(points, p1, p2, p3) > 0.;
        bool t2 = triangleArea(points, p1, p2, p4) > 0.;
        bool t3 = triangleArea(points, p3, p4, p1) > 0.;
        bool t4 = triangleArea(points, p3, p4, p2) > 0.;

        return (t1 != t2) && (t3 != t4);
    }
}

bool VertexHolder::isCollinear(const std::vector<Point> &points, size_t p1, size_t p2, size_t p3){
    double area = triangleArea(points, p1, p2, p3);

    double e12_2 = Vector(points[p1], points[p2]).sqr();
    double e23_2 = Vector(points[p2], points[p3]).sqr();
    double e13_2 = Vector(points[p1], points[p3]).sqr();

    double max_2 = std::max(e12_2, std::max(e23_2, e13_2));

    double eps = std::numeric_limits<double>::epsilon();
    if(max_2 <= eps || 2. * std::abs(area) <= eps * max_2){
        return true;
    }else{
        return false;
    }
}

double VertexHolder::triangleArea(const std::vector<Point> &points, size_t p1, size_t p2, size_t p3){
    return 0.5 * ((points[p2].x - points[p1].x) * (points[p3].y - points[p1].y) -
                  (points[p3].x - points[p1].x) * (points[p2].y - points[p1].y));
}

bool VertexHolder::isBetween(const std::vector<Point> &points, size_t p1, size_t p2, size_t p3){
    if(!isCollinear(points, p1, p2, p3)){
        return false;
    } else if(std::abs(points[p1].y - points[p2].y) < std::abs(points[p1].x - points[p2].x)){
        return std::min(points[p1].x, points[p2].x) <= points[p3].x && points[p3].x <= std::max(points[p1].x, points[p2].x);
    } else{
        return std::min(points[p1].y, points[p2].y) <= points[p3].y && points[p3].y <= std::max(points[p1].y, points[p2].y);
    }
}

bool VertexHolder::inCone(const std::vector<Point> &points, size_t p1, size_t p2, std::vector<size_t> &prevPoint, std::vector<size_t> &nextPoint){
    double t = triangleArea(points, p1, nextPoint[p1], prevPoint[p1]);

    if(t >= 0.){
        double t1 = triangleArea(points, p1, p2, prevPoint[p1]);
        double t2 = triangleArea(points, p2, p1, nextPoint[p1]);
        return (t1 > 0.) && (t2 > 0.);
    } else{
        double t1 = triangleArea(points, p1, p2, nextPoint[p1]);
        double t2 = triangleArea(points, p2, p1, prevPoint[p1]);
        return (t1 < 0.) || (t2 < 0.);
    }
}

void VertexHolder::draw(WindowHolder &window) const {
    window.draw(*vertices, &(*texture));
}

void VertexHolder::setVertex(sf::Vertex &vertex, const Point &point) {
    vertex.position = point.toSfVector();
    vertex.texCoords = point.toSfVector();
}

std::shared_ptr<sf::Texture> VertexHolder::getTexture() const {
    return texture;
}

void VertexHolder::setColor(sf::Color color) {
    for(int i = 0;i < vertices->getVertexCount();++i){
        (*vertices)[i].color = color;
    }
}
