#ifndef T5_PROJECTILE_H
#define T5_PROJECTILE_H

#include "EntityStorage.h"
#include "Alive.h"
#include "Rotatable.h"
#include "Movable.h"
#include "Edge.h"

class Projectile : virtual public Rotatable, virtual public Movable {
private:
    int m_damage;
    Polygon::Array &m_polygons;
    EntityStorage &m_entitiyStorage;

    static constexpr char DAMAGEKEY[] = "damage";

    bool checkHitAlive();

public:
    Projectile(const DataNode &dataNode, Polygon::Array &polygons_, EntityStorage &entityStorage);
    Projectile(const Projectile &copyFrom) = default;

    ~Projectile() override = default;

    bool mainUpdate() override;
};


#endif //T5_PROJECTILE_H
