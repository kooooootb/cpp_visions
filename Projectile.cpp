#include "Projectile.h"
#include "Polygon.h"
#include "Weapon.h"
#include "Player.h"

Projectile::Projectile(const Weapon &weapon, std::vector<std::shared_ptr<Player>> &Players, const KDPolygonsTree &tree, Polygons &Polygons) :
    Entity(projectileSprite, weapon.getPosition(), weapon.getAngle()) , speed(weapon.getSpeed()) , damage(weapon.getDamage()) ,
    direction(speed * std::cos(angle), speed * std::sin(angle)) , players(Players) , polygonsTree(tree), polygons(Polygons)
{
    position += ((direction / speed) * 2 * SHAPERADIUS);
}

bool Projectile::update() {//true if destroyed
    //check if it hits somebody
    bool hit;
    for(const auto &player : players){
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
                    players.erase(std::find(players.begin(), players.end(), player));
                }

                return true;
            }
        }
    }

    //check if able to move forward
    float query[2] = {position.x, position.y};
    std::vector<std::pair<uint32_t, float>> ret_matches;
    polygonsTree.radiusSearch(query, 3 * pow(speed, 2), ret_matches, {});

    polygons.hideAll();
    for(auto &pair : ret_matches){
        polygons[pair.first].polygon.makeVisible();
    }
    std::vector<Edge> edges = polygons.collectEdges();

    bool edgeFound = false;
    for(auto &edge : edges){
        if(lineVSLineIntersectionCheck(position, direction, edge.first, Vector(edge.first, edge.second))){
            edgeFound = true;

            break;
        }
    }

    if(!edgeFound){
        updatePosition();
    }

    return edgeFound;
}

void Projectile::updatePosition() {
    position += direction;

    sprite->setPosition(position.x, position.y);
}
