#include <fstream>

#include "Weapon.h"
#include "Player.h"
#include "Polygon.h"

Weapon::Weapon(std::vector<float> args) :
        Entity(args[0], args[1]) ,
        ammo((unsigned int) args[2]) , damage(args[3]) , speed(args[4])
{
    auto tempShape = std::make_shared<sf::CircleShape>(SHAPERADIUS);
    tempShape->setFillColor(defWeaponColor);
    tempShape->setPosition(position.x, position.y);
    tempShape->setOrigin(SHAPERADIUS, SHAPERADIUS);

    shape = tempShape;
}

Weapon::Weapon() :
        Entity(Point((float) screen_width / 2, (float) screen_height / 2)) ,
        ammo(AMMO) , damage(DAMAGE) , speed(BULLETSPEED)
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
    float temp;

    for(int i = 0; i < argWeaponLength; ++i){
        fd.read(reinterpret_cast<char *>(&temp), sizeof(temp));
        res.push_back(temp);
    }

    fd.close();

    return res;
}

bool Weapon::shoot(std::shared_ptr<Player> &target, Point &where, const std::vector<Edge> &blockingEdges) {//true if killed, not empty
    ammo -= 1;

    //check for inters is limited (0 < t < 1)
    //calc inters points is unlimited (0 < t)

    std::vector<Point> edgeHits = vectorVSEdgesIntersection(position, Vector(position, where), blockingEdges);
    Point edgeHit{};
    if(!edgeHits.empty()){
        edgeHit = findNearestPoint(position, edgeHits);
    }

    if(target == nullptr){//check where bullet will fall (unlimited)
        if(!edgeHits.empty()) {
            where = edgeHit;
        }

        return false;
    }else {//check if there is an obstacle (limited)
        if(vectorVSEdgesIntersectionCheck(position, Vector(position, target->getPosition()), blockingEdges)){
            where = edgeHit;

            return false;
        }else {
            return target->takeDamage(damage);
        }
    }
}

void Weapon::shoot() {
    if(ammo > 0){
        ammo -= 1;
    }
}

bool Weapon::spendBullet() {
    bool empty = ammo < 1;
    if(!empty){
        ammo -= 1;
    }
    return empty;
}
