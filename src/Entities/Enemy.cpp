#include "Enemy.h"

Enemy::Enemy(const DataNode &dataNode, Polygon::Array &polygons_, EntityStorage &entityStorage) :
        Alive(dataNode) , Movable(dataNode) , Armed(dataNode) , Visioning(dataNode) , Entity(dataNode) , Rotatable(dataNode) ,
        m_polygons(polygons_) , m_entityStorage(entityStorage)
{}

bool Enemy::mainUpdate() {
    if(isAlive()){
        move(m_polygons);
        regenerate();
        updateViewShape(m_polygons);
        updateSeeingEntities(m_entityStorage);

        return false;
    } else{
        return true;
    }
}
