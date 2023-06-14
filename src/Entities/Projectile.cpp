#include <cmath>
#include <utility>

#include "Projectile.h"
#include "Polygon.h"
#include "Intersections.h"

Projectile::Projectile(const DataNode &dataNode, Polygon::Array &polygons, EntityStorage &entityStorage) :
    Movable(dataNode) , Rotatable(dataNode) , Entity(dataNode) ,
    m_polygons(polygons) , m_entitiyStorage(entityStorage) , m_damage((int) dataNode.getNumber(DAMAGEKEY))
{}

bool Projectile::mainUpdate() {
    if(checkHitAlive()){
        return true;
    }

    if(move(m_polygons)){
        return true;
    }

    return false;
}

bool Projectile::checkHitAlive() {
    //check if it hits somebody
    bool hit;

    auto alives = m_entitiyStorage.iterateAlives();
    for(const auto &alivePair : alives){
        auto &alive = alivePair.second;
        Vector plVector(getPosition(), alive.getPosition());
        if(plVector.sqr() < std::pow(getMaxSpeed(), 2)){
            hit = Inter::circleVSLineCheck(getPosition(), getPosition() + getVelocity(), alive.getPosition(), alive.getBodyRadius());

            // need to check if line intersects with circle only 1 time TODO
            if(Vector(alive.getPosition(), getPosition()).sqr() < pow(alive.getBodyRadius(), 2)){
                hit = true;
            }
            if(Vector(alive.getPosition(), getPosition() + getVelocity()).sqr() < pow(alive.getBodyRadius(), 2)){
                hit = true;
            }

            if(hit){
                if(alive.takeDamage(m_damage)){
                    m_entitiyStorage.queueRemoveAlive(alivePair.first);
                }

                return true;
            }
        }
    }
    return false;
}
