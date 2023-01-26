#ifndef T5_PROJECTILE_H
#define T5_PROJECTILE_H

#include <vector>

#include "Entity.h"

class BasePlayer;

class Projectile : public Entity {
private:
    const float speed;
    const unsigned int damage;
    const Vector direction;//every tick pos += dir
    std::vector<std::shared_ptr<BasePlayer>> &players;
    const std::vector<std::shared_ptr<Polygon>> &polygons;

    void updatePosition();
public:
    Projectile(const Weapon &weapon, std::vector<std::shared_ptr<BasePlayer>> &Players, const std::vector<std::shared_ptr<Polygon>> &Polygons);

    bool update();
};


#endif //T5_PROJECTILE_H
