#ifndef T5_WEAPON_H
#define T5_WEAPON_H

#include <vector>

#include "Entity.h"

class Weapon : public Entity{
private:
    unsigned int ammo;
    float damage;


public:
    Weapon();
    Weapon(std::vector<float> args);

    static std::vector<float> loadEntity(const std::string &fname);

    bool shoot(std::shared_ptr<Player> &target);
};


#endif //T5_WEAPON_H
