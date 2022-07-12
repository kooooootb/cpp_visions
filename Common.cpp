#include <SFML/Graphics.hpp>

#include <list>
#include <vector>
#include <memory>
#include <cmath>
#include <iostream>
#include <fstream>

#include "Polygon.h"

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

void saveLevel(const std::string &fname, const std::vector<std::shared_ptr<std::list<sf::Vector2f>>> &polygons){
    std::ofstream fd(fname, std::ios::out | std::ios::binary | std::ios::trunc);

    if(fd.is_open()){
        auto size = polygons.size();
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

void loadLevel(std::vector<std::shared_ptr<sf::Shape>> &shapes, const std::string &fname, std::vector<std::shared_ptr<std::list<sf::Vector2f>>> &polygons){
    std::ifstream fd(fname, std::ios::in | std::ios::binary);

    if(fd.is_open()){
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
}

Polygons loadLevelForTree(const std::string &fname){
    std::ifstream fd(fname, std::ios::in | std::ios::binary);

    Polygons pols;

    if(fd.is_open()){
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