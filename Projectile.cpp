#include "Projectile.h"
#include "Polygon.h"
#include "Weapon.h"
#include "Player.h"

#include <cmath>

Projectile::Projectile(const Weapon &weapon, std::vector<std::shared_ptr<BasePlayer>> &Players, const std::vector<std::shared_ptr<Polygon>> &Polygons) :
    Entity(projectileSprite, weapon.getPosition(), weapon.getAngle()) , speed(weapon.getSpeed()) , damage(weapon.getDamage()) ,
    direction(speed * std::cos(angle), speed * std::sin(angle)) , players(Players) , polygons(Polygons)
{
    position += ((direction / speed) * 2 * SHAPERADIUS);
}

bool Projectile::update() {//true if destroyed
    //check if it hits somebody
    bool hit;

    for(auto plIt = players.cbegin(), plEnd = players.cend();plIt != plEnd;++plIt){
        const std::shared_ptr<BasePlayer> &player = *plIt;
        Vector plVector(position, player->getPosition());
        if(plVector.sqr() < pow(speed, 2)){
            hit = circleVSLineIntersectionCheck(position, position + direction, player->getPosition(), player->getRadius());
            if(Vector(player->getPosition(), position).sqr() < pow(player->getRadius(), 2)){
                hit = true;
            }
            if(Vector(player->getPosition(), position + direction).sqr() < pow(player->getRadius(), 2)){
                hit = true;
            }

            if(hit){
                if(player->takeDamage(damage)){
                    players.erase(plIt);
                }

                return true;
            }
        }
    }

    // check if able to move forward
    double checkDistance = speed * 2;

    Point maxPoint = position + Vector(checkDistance, checkDistance);
    Point minPoint = position - Vector(checkDistance, checkDistance);

    for(const auto &pol : polygons){
        // find polygons that can intersect
        if(pol->minPoint().x < maxPoint.x && pol->minPoint().y < maxPoint.y && pol->maxPoint().x > minPoint.x && pol->maxPoint().y > minPoint.y){
            auto end = pol->getPoints().end();
            auto first = end - 1;
            auto second = pol->getPoints().begin();

            while(second != end){
                // find if they really intersect
                if(lineVSLineIntersectionCheck(position, direction, *first, Vector(*first, *second))) {
                    return true;
                }

                first = second;
                ++second;
            }
        }
    }

    // update if not destroyed
    updatePosition();

    return false;
}

void Projectile::updatePosition() {
    position += direction;

    sprite->setPosition((float) position.x, (float) position.y);
}
