//
// Created by zavgm on 14.05.2023.
//

#ifndef VISIONS_ENEMY_H
#define VISIONS_ENEMY_H

#include "DataNode.h"
#include "Polygon.h"
#include "EntityStorage.h"
#include "Alive.h"
#include "Movable.h"
#include "Rotatable.h"
#include "Armed.h"
#include "Visioning.h"

class Enemy : virtual public Alive, virtual public Movable, virtual public Rotatable,
        virtual public Armed, virtual public Visioning  {
    Polygon::Array &m_polygons;
    EntityStorage &m_entityStorage;

public:
    Enemy(const DataNode &dataNode, Polygon::Array &polygons, EntityStorage &entityStorage);

    Enemy(const Enemy &) = default;
    Enemy(Enemy &&) = default;

    ~Enemy() override = default;

    bool mainUpdate() override;
};


#endif //VISIONS_ENEMY_H
