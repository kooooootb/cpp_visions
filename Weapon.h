#ifndef T5_WEAPON_H
#define T5_WEAPON_H

#include <vector>

#include "Entity.h"

class Weapon : public Entity{
private:
    unsigned int ammo;
    const float speed;
    const unsigned int damage;

public:
    Weapon(std::pair<std::string, std::vector<float>> args, const Point &point);

    unsigned int getDamage() const{ return damage; }
    float getSpeed() const{ return speed; }
    unsigned int getAmmo() const{ return ammo; }
    const std::string &getName() const{ return name; }

    std::string name;

    static std::pair<std::string, std::vector<float>> loadEntity(const std::string &fname);

    bool shoot(std::shared_ptr<Player> &target, Point &where, const std::vector<Edge> &blockingEdges);//true if killed, not empty
    void shoot();
    bool spendBullet();
    bool empty() const{ return ammo < 1; }
};

#endif //T5_WEAPON_H
