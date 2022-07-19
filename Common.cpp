#include <SFML/Graphics.hpp>

#include <list>
#include <vector>
#include <memory>
#include <cmath>
#include <iostream>
#include <fstream>

#include "Polygon.h"
#include "Common.h"


int initViewSector(std::shared_ptr<sf::ConvexShape> &viewShape, const Point &center, float viewDistance, float currentAngle, float viewAngle){
    if(viewAngle > currentAngle){
        viewAngle -= 2 * M_PI;
    }
    viewAngle = currentAngle - viewAngle;

	int dotsAmount = (int) (viewDistance * viewAngle) - 2;//dots in arc

    if(dotsAmount < 1){
        return 2;
    }

    sf::Vector2f first = viewShape->getPoint(0);
    sf::Vector2f second = viewShape->getPoint(1);

	viewShape->setPointCount(dotsAmount + 3);//1 point reserved for center

    viewShape->setPoint(0, first);
    viewShape->setPoint(1, second);

	float dAngle = viewAngle / (float) (dotsAmount + 1);
    currentAngle -= dAngle;

    int index = 0;
	for(;index < dotsAmount;++index){
		viewShape->setPoint(index + 2, sf::Vector2f((float) (center.x + viewDistance * cos((double) currentAngle)),
												(float) (center.y + viewDistance * sin((double) currentAngle))));
		
		currentAngle -= dAngle;
	}

    return dotsAmount + 2;
}

Point Point::operator+(const Vector &vector) const {
    return { x + vector.x, y + vector.y };
}

Point Point::operator-(const Vector &vector) const {
    return { x - vector.x, y - vector.y };
}

Point &Point::operator+=(const Vector &vector) {
    x += vector.x;
    y += vector.y;

    return *this;
}

Point &Point::operator+=(const sf::Vector2f &vector) {
    x += (float) vector.x;
    y += (float) vector.y;

    return *this;
}

Vector operator-(const Point &point, const sf::Vector2f &vector) {
    return { point.x - vector.x, point.y - vector.y };
}

Vector operator-(const Point &point, const sf::Vector2i &vector) {
    return { point.x - vector.x, point.y - vector.y };
}

Point calcClosestPoint(const Edge &edge, const Point &point){
    if (edge.first.x == edge.second.x){
        return { edge.first.x, point.y };
    }
    if (edge.first.y == edge.second.y){
        return { point.x, edge.first.x };
    }
    float m1 = (edge.second.y - edge.first.y) / (edge.second.x - edge.first.x);
    float m2 = -1 / m1;
    float x = (m1 * edge.first.x - m2 * point.x + point.y - edge.first.y) / (m1 - m2);
    return {
            x,
            m2 * (x - point.x) + point.y
    };
}

bool findClosestEdge(const std::vector<Edge> &edges, Edge &edge, const Point &point){
    if(edges.empty()){
        return false;
    }

    float minDistSqr = Vector(point, calcClosestPoint(edges.front(), point)).sqr();
    const Edge *edgePtr = &edges.front();

    for(auto it = edges.cbegin() + 1, endIt = edges.end();it < endIt;++it){
        float distSqr = Vector(point, calcClosestPoint(*it, point)).sqr();

        if(distSqr < minDistSqr){
            edgePtr = &(*it);
            minDistSqr = distSqr;
        }
    }

    edge = *edgePtr;
    return true;
}

std::vector<Point> loadWeaponSpawnpoints(std::ifstream &fd){
    unsigned long long length;
    fd.read(reinterpret_cast<char *>(&length), sizeof(length));
    length /= 2 * sizeof(float);

    std::vector<Point> res;

    for(int i = 0;i < length;++i){
        Point point;

        fd.read(reinterpret_cast<char *>(&point.x), sizeof(point.x));
        fd.read(reinterpret_cast<char *>(&point.y), sizeof(point.y));

        res.push_back(point);
    }

    return res;
}

std::pair<int, int> loadBoarders(std::ifstream &fd) {
    int width, height;

    fd.read(reinterpret_cast<char *>(&width), sizeof(width));
    fd.read(reinterpret_cast<char *>(&height), sizeof(height));

    return { width, height };
}

std::vector<Point> loadWeaponSpawnpoints(const std::string &fname){
    std::ifstream fd(fname, std::ios::in | std::ios::binary);

    std::vector<Point> res;

    if(fd.is_open()){
        unsigned long long length;
        fd.read(reinterpret_cast<char *>(&length), sizeof(length));
        length /= 2 * sizeof(float);

        for(int i = 0;i < length;++i){
            Point point;

            fd.read(reinterpret_cast<char *>(&point.x), sizeof(point.x));
            fd.read(reinterpret_cast<char *>(&point.y), sizeof(point.y));

            res.push_back(point);
        }

        fd.close();
    }


    return res;
}

std::pair<int, int> loadBoarders(const std::string &fname){
    std::ifstream fd(fname, std::ios::in | std::ios::binary);

    int width, height;

    if(fd.is_open()){
        skipWeaponSpawnpoints(fd);

        fd.read(reinterpret_cast<char *>(&width), sizeof(width));
        fd.read(reinterpret_cast<char *>(&height), sizeof(height));
    }


    return { width, height };
}

void saveLevel(const std::string &fname, const std::vector<std::shared_ptr<std::list<sf::Vector2f>>> &polygons,
               const std::vector<Point> &weaponSps, const std::pair<int, int> &boarders) {
    std::ofstream fd(fname, std::ios::out | std::ios::binary | std::ios::trunc);

    if(fd.is_open()){
        unsigned long long weaponsOffset = weaponSps.size() * 2 * sizeof(float);
        fd.write(reinterpret_cast<char *>(&weaponsOffset), sizeof(weaponsOffset));

        for(const auto &point : weaponSps){
            float x = point.x, y = point.y;

            fd.write(reinterpret_cast<char *>(&x), sizeof(x));
            fd.write(reinterpret_cast<char *>(&y), sizeof(y));
        }

        int width = boarders.first, height = boarders.second;
        fd.write(reinterpret_cast<char *>(&width), sizeof(width));
        fd.write(reinterpret_cast<char *>(&height), sizeof(height));

        unsigned long long size = polygons.size();
        fd.write(reinterpret_cast<char *>(&size), sizeof(size));

        for(auto &it : polygons){
            std::list<sf::Vector2f> &list = *it;
            size = list.size();
            fd.write(reinterpret_cast<char *>(&size), sizeof(size));

            for(auto &point : list){
                fd.write(reinterpret_cast<char *>(&point.x), sizeof(point.x));
                fd.write(reinterpret_cast<char *>(&point.y), sizeof(point.y));
            }
        }

        fd.close();
    }
}

bool loadLevel(std::vector<std::shared_ptr<sf::Shape>> &shapes, const std::string &fname,
               std::vector<std::shared_ptr<std::list<sf::Vector2f>>> &polygons, std::vector<Point> &weaponSps) {
    std::ifstream fd(fname, std::ios::in | std::ios::binary);

    if(fd.is_open()){
        weaponSps = loadWeaponSpawnpoints(fd);
        skipBoarders(fd);

        unsigned long long polygonsSize;
        fd.read(reinterpret_cast<char *>(&polygonsSize), sizeof(polygonsSize));

        for(int i = 0;i < polygonsSize;++i){
            polygons.emplace_back(std::make_shared<std::list<sf::Vector2f>>());

            std::list<sf::Vector2f> &list = *polygons.back();

            unsigned long long listSize;
            fd.read(reinterpret_cast<char *>(&listSize), sizeof(listSize));

            for(int j = 0;j < listSize;++j){
                float x, y;
                fd.read(reinterpret_cast<char *>(&x), sizeof(x));
                fd.read(reinterpret_cast<char *>(&y), sizeof(y));
                list.emplace_back(x, y);
            }
        }

        fd.close();
    }else{
        return false;
    }

    if(!weaponSps.empty()){
        for(const auto &it : weaponSps){
            std::shared_ptr<sf::CircleShape> circle = std::make_shared<sf::CircleShape>(3);
            circle->setFillColor(sf::Color::Green);
            circle->setPosition(it.x, it.y);
            circle->setOrigin(3, 3);

            shapes.push_back(circle);
        }
    }

    if(!polygons.empty()){
        for(const auto &it : polygons){
            const std::list<sf::Vector2f> &list = *it;

            sf::ConvexShape convex;
            convex.setPointCount(list.size());
            convex.setFillColor(sf::Color::Black);
            int i = 0;
            for(const auto &point : list){
                convex.setPoint(i++, point);
            }
            shapes.push_back(std::make_shared<sf::ConvexShape>(convex));
        }
    }

    return true;
}

void skipWeaponSpawnpoints(std::ifstream &fd){
    unsigned long long offset;
    fd.read(reinterpret_cast<char *>(&offset), sizeof(offset));

    fd.seekg(offset, std::ifstream::cur);
}

void skipBoarders(std::ifstream &fd){
    fd.seekg(2 * sizeof(int), std::ifstream::cur);
}

Polygons loadLevelForTree(const std::string &fname){
    std::ifstream fd(fname, std::ios::in | std::ios::binary);

    Polygons pols;

    if(fd.is_open()){
        skipWeaponSpawnpoints(fd);
        skipBoarders(fd);

        unsigned long long polygonsAmount;
        fd.read(reinterpret_cast<char *>(&polygonsAmount), sizeof(polygonsAmount));

        for(int i = 0; i < polygonsAmount; ++i){
            std::shared_ptr<Polygon> pol = std::make_shared<Polygon>();

            unsigned long long bpAmount;
            fd.read(reinterpret_cast<char *>(&bpAmount), sizeof(bpAmount));

            for(int j = 0; j < bpAmount; ++j){
                float x, y;
                fd.read(reinterpret_cast<char *>(&x), sizeof(x));
                fd.read(reinterpret_cast<char *>(&y), sizeof(y));
                pol->addBPoint(PolygonPoint(*pol, x, y));
            }
            pol->endAdding();

            pols.add(pol);
        }

        fd.close();
    }

    if(!pols.empty()){
        for(auto &it : pols){
            it->setConvex();
        }
    }

    return pols;
}

Point findNearestPoint(const Point &center, const std::vector<Point> &points){
    float distSqr = std::numeric_limits<float>::max(), newDistSqr;
    Point res(0, 0);

    for(const auto &point : points){
        newDistSqr = Vector(center, point).sqr();
        if(newDistSqr < distSqr){
            res = point;
            distSqr = newDistSqr;
        }
    }

    return res;
}

std::vector<Point> arcSegmentVSLineIntersection(const Point &p1, const Point &p2, const Point &center, float radius, const std::array<Vector, 3> &views){
    //|X^2 - center^2| = radius^2
    //X(t) = p1 + td , (t from 0 to 1)
    //delta = p1 - center
    Vector d(p1, p2);
    Vector delta(center, p1);

    std::vector<Point> res;

    auto isInSight = [&](const Point &point) -> bool {
        Vector pV(center, point);
        if(pV * views[1] < 0){
            return false;
        }else if(views[0].cross(pV) * pV.cross(views[2]) > 0){
            return true;
        }else{
            return false;
        }
    };

    float discriminant = (d * delta) * (d * delta) - d.sqr() * (delta.sqr() - radius * radius);

    if(discriminant < 0){
        return res;
    }else if(discriminant == 0){
        float t = (-1 * (d * delta)) / (d.sqr());
        if(t >= 0 && t <= 1){
            Point p(p1 + t * d);
            if(isInSight(p)){
                res.push_back(p);
            }
        }
        return res;
    }else{
        discriminant = std::sqrt(discriminant);

        float t;
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

std::vector<Point> circleVSLineIntersection(const Point &p1, const Point &p2, const Point &center, float radius){//vector is limited (p2)
    //|X^2 - center^2| = radius^2
    //X(t) = p1 + td , (t from 0 to 1)
    //delta = p1 - center
    Vector d(p1, p2);
    Vector delta(center, p1);

    std::vector<Point> res;

    float discriminant = (d * delta) * (d * delta) - d.sqr() * (delta.sqr() - radius * radius);

    if(discriminant < 0){
        return res;
    }else if(discriminant == 0){
        float t = (-1 * (d * delta)) / (d.sqr());
        if(t >= 0){
            Point p(p1 + t * d);
            res.push_back(p);
        }
        return res;
    }else{
        discriminant = std::sqrt(discriminant);

        float t;
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

bool circleVSLineIntersectionCheck(const Point &p1, const Point &p2, const Point &center, float radius){//check if line segment intersects circle 2 times
    //if line segment can intersect circle only 1 time check if 1 of its points inside circle
    Vector d(p1, p2);
    Vector delta(center, p1);

    std::vector<Point> res;

    float discriminant = (d * delta) * (d * delta) - d.sqr() * (delta.sqr() - radius * radius);

    if(discriminant < 0){
        return false;
    }else{
        float t = (-1 * (d * delta)) / (d.sqr());
        if(t >= 0 && t <= 1){
            return true;
        }
        else{
            return false;
        }
    }
}

std::vector<Point> twoLinesVSLineIntersection(const Point &p1, const Point &p2, const Point &center, const std::array<Vector, 3> &views){
    //1 line: p1 + s * d0
    const Vector d0(p1, p2);
    //2 line: center + t * views[0]
    //3 line: center + t * views[2]
    //p1 == P0 ; center == P1

    const Vector delta(p1, center);

    std::vector<Point> res;

    for(int i = 0;i < 3;i += 2){//for views[0] and views[2]
        const Vector &d1 = views[i];
        float denominator = d0 * (d1.perpendicular());

        if(denominator != 0){
            float t = (delta * (d0.perpendicular())) / denominator;
            float s = (delta * (d1.perpendicular())) / denominator;
            if(t >= 0 && t <= 1 && s >= 0 && s <= 1){
                Point cross = center + (t * d1);
                res.emplace_back(cross);
            }
        }
    }

    return res;
}

bool unlimitedLineVSLineIntersection(const Point &p1, const Vector &d0, const Point &p3, const Vector &d1, Point &res){//d0 is unlimited
    const Vector delta(p1, p3);

    bool hit = false;

    float denominator = d0 * (d1.perpendicular());

    if(denominator != 0){
        float t = (delta * (d0.perpendicular())) / denominator;
        float s = (delta * (d1.perpendicular())) / denominator;
        if(t >= 0 && t <= 1 && s >= 0){
            res = p3 + (t * d1);
            hit = true;
        }
    }

    return hit;
}

bool lineVSLineIntersection(const Point &p1, const Vector &d0, const Point &p3, const Vector &d1, Point &res){//d0 is unlimited
    const Vector delta(p1, p3);

    bool hit = false;

    float denominator = d0 * (d1.perpendicular());

    if(denominator != 0){
        float t = (delta * (d0.perpendicular())) / denominator;
        float s = (delta * (d1.perpendicular())) / denominator;
        if(t >= 0 && t <= 1 && s >= 0 && s <= 1){
            res = p3 + (t * d1);
            hit = true;
        }
    }

    return hit;
}

bool lineVSLineIntersectionCheck(const Point &p1, const Vector &d0, const Point &p3, const Vector &d1){//only checks if its intersecting
    const Vector delta(p1, p3);

    bool hit = false;

    float denominator = d0 * (d1.perpendicular());

    if(denominator != 0){
        float t = (delta * (d0.perpendicular())) / denominator;
        float s = (delta * (d1.perpendicular())) / denominator;
        if(t >= 0 && t <= 1 && s >= 0 && s <= 1){
            hit = true;
        }
    }

    return hit;
}

bool vectorVSEdgesIntersectionCheck(const Point &center, const Vector &vector, const std::vector<Edge> &edges){
    bool hit = false;

    for (const auto &edge: edges) {
        if (lineVSLineIntersectionCheck(center, vector, edge.first, Vector(edge.first, edge.second))) {
            hit = true;
        }
    }

    return hit;
}

std::vector<Point> vectorVSEdgesIntersection(const Point &center, const Vector &vector, const std::vector<Edge> &edges){
    Point temp;
    std::vector<Point> res;

    for (const auto &edge: edges) {
        if (lineVSLineIntersection(center, vector, edge.first, Vector(edge.first, edge.second), temp)) {
            res.push_back(temp);
        }
    }

    return res;
}

void writeFname(std::string &fname, std::ofstream &fd){
    unsigned long long size = fname.size();
    fd.write(reinterpret_cast<char *>(&size), sizeof(size));
    fd.write(reinterpret_cast<char *>(&fname[0]), sizeof(fname[0]) * size);
}

void writeFname(std::ofstream &fd){
    std::string fname;
    std::cout << "Input entity's sprite file name:";
    std::cin >> fname;

    writeFname(fname, fd);
}

std::string readFname(std::ifstream &fd){
    std::string res;

    unsigned long long size;

    fd.read(reinterpret_cast<char *>(&size), sizeof(size));

    res.resize(size);

    fd.read(reinterpret_cast<char *>(&res[0]), sizeof(char) * size);

    return res;
}

sf::Vector2f getMousePosition(const sf::RenderWindow &window){
    return window.mapPixelToCoords(sf::Mouse::getPosition(window));
}