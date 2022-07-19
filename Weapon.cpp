#include <fstream>

#include "Weapon.h"
#include "Player.h"
#include "Polygon.h"

Weapon::Weapon(std::pair<std::string, std::vector<float>> args, const Point &point) :
        Entity(args.first, point) ,
        ammo((unsigned int) args.second[0]) , damage(args.second[1]) , speed(args.second[2])
{
    name = args.first;
    name.erase(name.length() - 4);//delete ".png"

    setAngle(degToRad(rand() % 360));
}

std::pair<std::string, std::vector<float>> Weapon::loadEntity(const std::string &fname) {
    std::vector<float> res;
    std::string name;

    std::ifstream fd(fname);
    if(!fd.is_open()){
        return { name, res };
    }

    float temp;

    for(int i = 0; i < argWeaponLength; ++i){
        fd.read(reinterpret_cast<char *>(&temp), sizeof(temp));
        res.push_back(temp);
    }

    name = readFname(fd);

    fd.close();

    return { name, res };
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
