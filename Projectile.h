#ifndef T5_PROJECTILE_H
#define T5_PROJECTILE_H

#include "Entity.h"

class Projectile : public Entity {
private:
    const float speed, damage;
    const Vector direction;//every tick pos += dir
    std::vector<std::shared_ptr<Player>> &players;
    const KDPolygonsTree &polygonsTree;
    Polygons &polygons;

    void updatePosition();
public:
    Projectile(const Weapon &weapon, std::vector<std::shared_ptr<Player>> &Players, const KDPolygonsTree &polygonsTree, Polygons &Polygons);

    bool update();


};


#endif //T5_PROJECTILE_H
