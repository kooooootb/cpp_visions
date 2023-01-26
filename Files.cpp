#include "Common.h"
#include "Polygon.h"

#include <vector>
#include <fstream>
#include <exception>

//skip weapons
void skWp(std::ifstream &fd) {
    size_t offset;
    fd.read(reinterpret_cast<char *>(&offset), sizeof(offset));

    fd.seekg(offset, std::ifstream::cur);
}

//skip boarders
void skBd(std::ifstream &fd) {
    fd.seekg(2 * sizeof(int), std::ifstream::cur);
}

//save weapon spawnpoints
bool svWp(std::ofstream &fd, const std::vector<Point> &weaponSps){
    size_t weaponsOffset = weaponSps.size() * 2 * sizeof(double);
    fd.write(reinterpret_cast<char *>(&weaponsOffset), sizeof(weaponsOffset));

    for (const auto &point: weaponSps) {
        double x = point.x, y = point.y;

        fd.write(reinterpret_cast<char *>(&x), sizeof(x));
        fd.write(reinterpret_cast<char *>(&y), sizeof(y));
    }

    return true;
}

//save boarders
bool svBd(std::ofstream &fd, const std::pair<int, int> &bd){
    int width = bd.first, height = bd.second;

    fd.write(reinterpret_cast<char *>(&width), sizeof(width));
    fd.write(reinterpret_cast<char *>(&height), sizeof(height));

    return true;
}

//save polygons
bool svPl(std::ofstream &fd, const std::vector<std::shared_ptr<std::list<sf::Vector2f>>> &pl){
    size_t size = pl.size();
    fd.write(reinterpret_cast<char *>(&size), sizeof(size));

    for (auto &it: pl) {
        std::list<sf::Vector2f> &list = *it;
        size = list.size();
        fd.write(reinterpret_cast<char *>(&size), sizeof(size));

        for (auto &point: list) {
            fd.write(reinterpret_cast<char *>(&point.x), sizeof(point.x));
            fd.write(reinterpret_cast<char *>(&point.y), sizeof(point.y));
        }
    }

    return true;
}

//save polygons weapons and boarders
void svLv(const std::string &fname, const std::vector<std::shared_ptr<std::list<sf::Vector2f>>> &pl,
              const std::vector<Point> &wp, const std::pair<int, int> &bd)
{
    std::ofstream fd(fname, std::ios::out | std::ios::binary | std::ios::trunc);

    if (fd.is_open()) {
        svWp(fd, wp);
        svBd(fd, bd);
        svPl(fd, pl);

        fd.close();
    }
}

// load weapon spawn points
std::vector<Point> ldWp(std::ifstream &fd) {
    size_t length;
    fd.read(reinterpret_cast<char *>(&length), sizeof(length));
    length /= 2 * sizeof(double);

    std::vector<Point> res;

    for (int i = 0; i < length; ++i) {
        Point point(0, 0);

        fd.read(reinterpret_cast<char *>(&point.x), sizeof(point.x));
        fd.read(reinterpret_cast<char *>(&point.y), sizeof(point.y));

        res.push_back(point);
    }

    return res;
}

//load weapon spawn points wrapper
std::vector<Point> ldWpWp(const std::string &fname) {
    std::ifstream fd(fname, std::ios::in | std::ios::binary);

    if (fd.is_open()) {
        return loadWeaponSpawnpoints(fd);
    } else {
        throw std::runtime_error("File not found: " + fname);
    }
}

//load boarders
std::pair<int, int> ldBd(std::ifstream &fd) {
    int width, height;

    fd.read(reinterpret_cast<char *>(&width), sizeof(width));
    fd.read(reinterpret_cast<char *>(&height), sizeof(height));

    return { width, height };
}

//load boarders wrapper
std::pair<int, int> ldBdWp(const std::string &fname) {
    std::ifstream fd(fname, std::ios::in | std::ios::binary);

    if (fd.is_open()) {
        skWp(fd);
        return ldBd(fd);
    } else {
        throw std::runtime_error("File not found " + fname);
    }
}

//load polygons
//bool ldPl(const std::string &fname, std::vector<std::shared_ptr<std::list<sf::Vector2f>>> &polygons) {
//    std::ifstream fd(fname, std::ios::in | std::ios::binary);
//
//    if (fd.is_open()) {
//        skWp(fd);
//        skBd(fd);
//
//        size_t polygonsSize;
//        fd.read(reinterpret_cast<char *>(&polygonsSize), sizeof(polygonsSize));
//
//        for (int i = 0; i < polygonsSize; ++i) {
//            polygons.emplace_back(std::make_shared<std::list<sf::Vector2f>>());
//
//            std::list<sf::Vector2f> &list = *polygons.back();
//
//            size_t listSize;
//            fd.read(reinterpret_cast<char *>(&listSize), sizeof(listSize));
//
//            for (int j = 0; j < listSize; ++j) {
//                double x, y;
//                fd.read(reinterpret_cast<char *>(&x), sizeof(x));
//                fd.read(reinterpret_cast<char *>(&y), sizeof(y));
//                list.emplace_back(x, y);
//            }
//        }
//
//        fd.close();
//    } else {
//        throw std::runtime_error("File not found: " + fname);
//    }
//
//    if (!weaponSps.empty()) {
//        for (const auto &it: weaponSps) {
//            std::shared_ptr<sf::CircleShape> circle = std::make_shared<sf::CircleShape>(3);
//            circle->setFillColor(sf::Color::Green);
//            circle->setPosition(it.x, it.y);
//            circle->setOrigin(3, 3);
//
//            shapes.push_back(circle);
//        }
//    }
//
//    if (!polygons.empty()) {
//        for (const auto &it: polygons) {
//            const std::list<sf::Vector2f> &list = *it;
//
//            sf::ConvexShape convex;
//            convex.setPointCount(list.size());
//            convex.setFillColor(sf::Color::Black);
//            int i = 0;
//            for (const auto &point: list) {
//                convex.setPoint(i++, point);
//            }
//            shapes.push_back(std::make_shared<sf::ConvexShape>(convex));
//        }
//    }
//
//    return true;
//}

//load polygons
std::vector<std::shared_ptr<Polygon>> ldPl(std::ifstream &fd) {
    std::vector<std::shared_ptr<Polygon>> polygons;

    size_t plAmount;
    fd.read(reinterpret_cast<char *>(&plAmount), sizeof(plAmount));
    polygons.reserve(plAmount);

    double minx, maxx;
    double miny, maxy;

    for (int i = 0; i < plAmount; ++i) { // for every polygon
        minx = std::numeric_limits<double>::max(), maxx = std::numeric_limits<double>::min();
        miny = std::numeric_limits<double>::max(), maxy = std::numeric_limits<double>::min();

        std::vector<Point> points;

        size_t bpAmount;
        fd.read(reinterpret_cast<char *>(&bpAmount), sizeof(bpAmount));

        points.reserve(bpAmount);

        for (int j = 0; j < bpAmount; ++j) { // for every point in polygon
            double x, y;
            fd.read(reinterpret_cast<char *>(&x), sizeof(x));
            fd.read(reinterpret_cast<char *>(&y), sizeof(y));
            points.emplace_back(x, y);

            // make bbox
            if (x > maxx) {
                maxx = x;
            } else if (x < minx) {
                minx = x;
            }

            if (y > maxy) {
                maxy = y;
            } else if (y < miny) {
                miny = y;
            }
        }

        polygons.push_back(std::make_shared<Polygon>(points, Point(minx, miny), Point(maxx, maxy)));
        polygons.back()->setConvex();
    }

    return polygons;
}

//load polygons wrapper
std::vector<std::shared_ptr<Polygon>> ldPlWp(const std::string &fname) {
    std::ifstream fd(fname, std::ios::in | std::ios::binary);

    if (fd.is_open()) {
        skWp(fd);
        skBd(fd);

        return ldPl(fd);
    } else{
        throw std::runtime_error("File not found " + fname);
    }
}