#include "Common.h"
#include "Polygon.h"
#include "Player.h"
#include <utility>

Polygon::Polygon(std::vector<Point> points, Point minPoint, Point maxPoint) :
        points(std::move(points)) , bbox(std::make_pair(minPoint, maxPoint)) , viewVertices(sf::Triangles) {
    setViewShape();
}

Polygon::Polygon(std::vector<Point> points) : points(std::move(points)) , viewVertices(sf::Triangles) {
    setBBox();
    setViewShape();
}

void Polygon::setBBox(){
    double maxx, maxy, minx, miny;
    for(const auto &point : this->points){
        maxx = std::max(point.x, maxx);
        maxy = std::max(point.y, maxy);
        minx = std::min(point.x, minx);
        miny = std::min(point.y, miny);
    }
    bbox = std::make_pair(Point(minx, miny), Point(maxx, maxy));
}

void Polygon::addPoint(const Point &point) {
	points.push_back(point);

    setViewShape();
}

// delaunay triangulation
//void Polygon::setViewShape() {
//    // find min max points
//    double minX = points[0].x;
//    double minY = points[0].y;
//    double maxX = minX;
//    double maxY = minY;
//
//    for(const Point &point : points){
//        minX = std::min(point.x, minX);
//        minY = std::min(point.y, minY);
//        maxX = std::max(point.x, maxX);
//        maxY = std::max(point.y, maxY);
//    }
//
//    // configure parameters for super triangle
//    const double deltaMax = std::max(maxX - minX, maxY - minY);
//    const double midX = (maxX + minX) / 2;
//    const double midY = (maxY + minY) / 2;
//
//    // set points for super triangle
//    const Point p1(midX - 20 * deltaMax, midY - deltaMax);
//    const Point p2(midX, midY + 20 * deltaMax);
//    const Point p3(midX + 20 * deltaMax, midY - deltaMax);
//
//    // set super triangle
//    std::vector<Triangle> triangles{Triangle(p1, p2, p3)};
//
//    // struct keeps edge and its badness
//    struct DetEdge{
//        Edge edge;
//        bool isBad;
//
//        DetEdge(const Point &a, const Point &b) : edge(a, b) , isBad(false) {}
//    };
//
//    // handle each point
//    for(const Point &point : points){
//        std::vector<DetEdge> polygon;
//
//        for(Triangle &tr : triangles){
//            if(tr.circumCircleContains(point)){
//                tr.isBad = true;
//                polygon.emplace_back(tr.a, tr.b);
//                polygon.emplace_back(tr.b, tr.c);
//                polygon.emplace_back(tr.c, tr.a);
//            }
//        }
//
//        triangles.erase(std::remove_if(triangles.begin(), triangles.end(), [](const Triangle &tr) -> bool {
//            return tr.isBad;
//        }), triangles.end());
//
//        for(auto e1 = polygon.begin();e1 != polygon.end();++e1){
//            for(auto e2 = e1 + 1;e2 != polygon.end();++e2){
//                if(almost_equal(e1->edge, e2->edge)){
//                    e1->isBad = true;
//                    e2->isBad = true;
//                }
//            }
//        }
//
//        polygon.erase(std::remove_if(polygon.begin(), polygon.end(), [](const DetEdge &dedge) -> bool {
//            return dedge.isBad;
//        }), polygon.end());
//
//        for(const DetEdge &dedge : polygon){
//            triangles.emplace_back(dedge.edge.first, dedge.edge.second, point);
//        }
//    }
//
//    triangles.erase(std::remove_if(triangles.begin(), triangles.end(), [&p1, &p2, &p3](const Triangle &tr) -> bool {
//        return tr.containsPoint(p1) || tr.containsPoint(p2) || tr.containsPoint(p3);
//    }), triangles.end());
//
//    viewVertices.resize(triangles.size() * 3);
//    int i = 0;
//    for(const Triangle &tr : triangles){
//        viewVertices[i++] = tr.a.toSfVector();
//        viewVertices[i++] = tr.b.toSfVector();
//        viewVertices[i++] = tr.c.toSfVector();
//    }
//}

void Polygon::setViewShape() {
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
        isEars[i] = isInternalDiagonal(prevPoint[i], nextPoint[i], prevPoint, nextPoint);
    }

    int triangleNum = 0;
    size_t i0, i1, i2, i3, i4;
    i2 = 0;
    size_t prevWorked = i2;
    bool didTwice = false;
    viewVertices.resize(3 * (n - 2));
    while(triangleNum < n - 3){
        if(isEars[i2]){
            i1 = prevPoint[i2];
            i0 = prevPoint[i1];
            i3 = nextPoint[i2];
            i4 = nextPoint[i3];

            prevPoint[i3] = i1;
            nextPoint[i1] = i3;

            isEars[i1] = isInternalDiagonal(i0, i3, prevPoint, nextPoint);
            isEars[i2] = false;
            isEars[i3] = isInternalDiagonal(i1, i4, prevPoint, nextPoint);

            viewVertices[triangleNum * 3 + 0] = points[i3].toSfVector();
            viewVertices[triangleNum * 3 + 1] = points[i1].toSfVector();
            viewVertices[triangleNum * 3 + 2] = points[i2].toSfVector();
            triangleNum += 1;

            prevWorked = nextPoint[i2];
            didTwice = false;
        } else{
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
                        isEars[i] = isInternalDiagonal(prevPoint[i], nextPoint[i], prevPoint, nextPoint);
                    }
                    didTwice = true;
                }
            }
        }

        i2 = nextPoint[i2];
    }

    // Add remaining vertices
    viewVertices[triangleNum * 3 + 0] = points[i2].toSfVector();
    viewVertices[triangleNum * 3 + 1] = points[nextPoint[i2]].toSfVector();
    viewVertices[triangleNum * 3 + 2] = points[prevPoint[i2]]   .toSfVector();
}

void Polygon::draw(sf::RenderWindow &window) const {
    window.draw(viewVertices, &viewTexture);
}

bool Polygon::isInternalDiagonal(size_t p1, size_t p2, std::vector<size_t> &prevPoint, std::vector<size_t> &nextPoint) {
    return inCone(p1, p2, prevPoint, nextPoint) && inCone(p2, p1, prevPoint, nextPoint) && isDiagonal(p1, p2, nextPoint);
}

bool Polygon::isDiagonal(size_t p1, size_t p2, std::vector<size_t> &nextPoint) {
    size_t p = p1, pn = nextPoint[p];

    do{
        if(p != p1 && p != p2 && pn != p1 && pn != p2){
            if(isIntersecting(p1, p2, p, pn)){
                return false;
            }
        }

        p = pn;
        pn = nextPoint[pn];
    }while(p != p1);

    return true;
}

bool Polygon::isIntersecting(size_t p1, size_t p2, size_t p3, size_t p4){
    if(isProperlyIntersecting(p1, p2, p3, p4)){
        return true;
    } else if(isBetween(p1, p2, p3) || isBetween(p1, p2, p4) || isBetween(p3, p4, p1) || isBetween(p3, p4, p2)) {
        return true;
    } else{
        return false;
    }
}

bool Polygon::isProperlyIntersecting(size_t p1, size_t p2, size_t p3, size_t p4){
    if(isCollinear(p1, p2, p3) || isCollinear(p1, p2, p4) || isCollinear(p3, p4, p1) || isCollinear(p3, p4, p2)){
        return false;
    } else{
        bool t1 = triangleArea(p1, p2, p3) > 0.;
        bool t2 = triangleArea(p1, p2, p4) > 0.;
        bool t3 = triangleArea(p3, p4, p1) > 0.;
        bool t4 = triangleArea(p3, p4, p2) > 0.;

        return (t1 != t2) && (t3 != t4);
    }
}

bool Polygon::isCollinear(size_t p1, size_t p2, size_t p3){
    double area = triangleArea(p1, p2, p3);

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

double Polygon::triangleArea(size_t p1, size_t p2, size_t p3){
    return 0.5 * ((points[p2].x - points[p1].x) * (points[p3].y - points[p1].y) -
        (points[p3].x - points[p1].x) * (points[p2].y - points[p1].y));
}

bool Polygon::isBetween(size_t p1, size_t p2, size_t p3){
    if(!isCollinear(p1, p2, p3)){
        return false;
    } else if(std::abs(points[p1].y - points[p2].y) < std::abs(points[p1].x - points[p2].x)){
        return std::min(points[p1].x, points[p2].x) <= points[p3].x && points[p3].x <= std::max(points[p1].x, points[p2].x);
    } else{
        return std::min(points[p1].y, points[p2].y) <= points[p3].y && points[p3].y <= std::max(points[p1].y, points[p2].y);
    }
}

bool Polygon::inCone(size_t p1, size_t p2, std::vector<size_t> &prevPoint, std::vector<size_t> &nextPoint){
    double t = triangleArea(p1, nextPoint[p1], prevPoint[p1]);

    if(t >= 0.){
        double t1 = triangleArea(p1, p2, prevPoint[p1]);
        double t2 = triangleArea(p2, p1, nextPoint[p1]);
        return (t1 > 0.) && (t2 > 0.);
    } else{
        double t1 = triangleArea(p1, p2, nextPoint[p1]);
        double t2 = triangleArea(p2, p1, prevPoint[p1]);
        return (t1 < 0.) || (t2 < 0.);
    }
}
