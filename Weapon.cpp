#include <fstream>

#include "Weapon.h"
#include "Player.h"

Weapon::Weapon(std::vector<float> args) :
        Entity(Point((float) screen_width / 2, (float) screen_height / 2)) ,
        ammo((unsigned int) args[0]) , damage(args[1])
{
    auto tempShape = std::make_shared<sf::CircleShape>(SHAPERADIUS);
    tempShape->setFillColor(defWeaponColor);
    tempShape->setPosition(position.x, position.y);
    tempShape->setOrigin(SHAPERADIUS, SHAPERADIUS);

    shape = tempShape;
}

Weapon::Weapon() :
        Entity(Point((float) screen_width / 2, (float) screen_height / 2)) ,
        ammo(AMMO) , damage(DAMAGE)
{
    auto tempShape = std::make_shared<sf::CircleShape>(SHAPERADIUS);
    tempShape->setFillColor(defWeaponColor);
    tempShape->setPosition(position.x, position.y);
    tempShape->setOrigin(SHAPERADIUS, SHAPERADIUS);

    shape = tempShape;
}

std::vector<float> Weapon::loadEntity(const std::string &fname) {
    std::ifstream fd(fname);
    if(!fd.is_open()){
        throw std::exception();
    }

    std::vector<float> res;
    float floatTemp;
    int intTemp;

    fd.read(reinterpret_cast<char *>(&intTemp), sizeof(intTemp));
    res.push_back(intTemp);

    fd.read(reinterpret_cast<char *>(&floatTemp), sizeof(floatTemp));
    res.push_back(floatTemp);

    fd.close();

    return res;
}

bool Weapon::shoot(std::shared_ptr<Player> &target) {
    if(ammo < 1){
        return false;
    }else{
        ammo -= 1;

        return target->takeDamage(damage);
    }
}
