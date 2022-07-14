#include <iostream>
#include <fstream>

#include "Player.h"
#include "Polygon.h"
#include "Weapon.h"
#include "Projectile.h"

Player::Player(std::vector<float> args) :
        Entity(args[0], args[1]) ,
        friction(args[2]) , step(args[3]) , viewDistance(args[4]) , shapeRadius(args[5]) , viewAngle(degToRad(args[6])) ,
        maxHealth(args[7]) , maxSpeed(args[8])
{
    health = maxHealth;

    auto playerShape = std::make_shared<sf::CircleShape>(shapeRadius);
    playerShape->setFillColor(defPlayerColor);
    playerShape->setPosition(position.x, position.y);
    playerShape->setOrigin(shapeRadius, shapeRadius);

    shape = playerShape;
}

Player::Player() :
    Entity(Point((float) screen_width / 2, (float) screen_height / 2)) ,
    friction(FRICTION) , step(STEP) , viewDistance(VIEWDISTANCE) , shapeRadius(SHAPERADIUS) , viewAngle(degToRad(VIEWANGLEDEG)) ,
    maxHealth(MAXHEALTH) , maxSpeed(SPEEDLIMIT)
{
    health = maxHealth;

    auto playerShape = std::make_shared<sf::CircleShape>(shapeRadius);
    playerShape->setFillColor(defPlayerColor);
    playerShape->setPosition(position.x, position.y);
    playerShape->setOrigin(shapeRadius, shapeRadius);

    shape = playerShape;
}

void Player::update(Polygons &polygons, KDPolygonsTree &tree, const sf::Vector2i &mousePos, const std::vector<std::shared_ptr<Entity>> &entities,
                    std::list<std::shared_ptr<sf::Shape>> &viewShape
#ifdef T5_DEBUG
               , sf::RenderWindow &window
#endif
               ) {
    //rotate view sector:
    setAngle(getAngleToZero(sf::Vector2f(mousePos.x - position.x, mousePos.y - position.y)));

    //update speed
    updateSpeed();

    //check for collision:
    collisionCheck(polygons, tree);

    //update positions
    updatePosition();

    //set forms' shapes:
    updateView(polygons, tree, entities, viewShape);

    //make player visible
    setVisibility(true);
}

void Player::updateSpeed() {
    float d = sqrt(pow(dx, 2) + pow(dy, 2));
    float frictionX = dx * friction / d;
    float frictionY = dy * friction / d;
    dx = std::abs(dx) > frictionX ? dx - frictionX : 0;
    dy = std::abs(dy) > frictionY ? dy - frictionY : 0;

    d = sqrt(pow(dx, 2) + pow(dy, 2));
    if(d > maxSpeed){
        dx /= d / maxSpeed;
        dy /= d / maxSpeed;
    }
}

std::vector<float> Player::loadEntity(const std::string &fname) {
    std::ifstream fd(fname);
    if(!fd.is_open()){
        throw std::exception();
    }

    std::vector<float> res;
    float temp;

    for(int i = 0; i < argPlayerLength; ++i){
        fd.read(reinterpret_cast<char *>(&temp), sizeof(temp));
        res.push_back(temp);
    }

    fd.close();

    return res;
}

bool Player::shoot(std::vector<std::shared_ptr<Player>> &players, std::shared_ptr<Projectile> &projectile, const KDPolygonsTree &tree, Polygons &polygons){//true if able to shoot
    if(activeWeapon == nullptr || activeWeapon->spendBullet()){
        return false;
    }

    projectile = std::make_shared<Projectile>(*activeWeapon, players, tree, polygons);

    return true;
}

void Player::pickWeapon(KDWeaponsTree &tree, std::vector<std::shared_ptr<Weapon>> &weapons) {
    if(activeWeapon == nullptr){//active weapon should be clear anyway
        const float query[2] = { position.x, position.y };
        size_t ret_index;
        float out_dist_sqr;

        nanoflann::KNNResultSet<float> resultSet(1);//search for 1 element
        resultSet.init(&ret_index, &out_dist_sqr);
        bool found = tree.findNeighbors(resultSet, query, {10});
        if(found){
            if(out_dist_sqr < PICKUPRADIUSSQR){
                activeWeapon = weapons[ret_index];
                weapons.erase(std::find(weapons.begin(), weapons.end(), activeWeapon));

                tree.removePoint(ret_index);
            }
        }
    }
}

void Player::changeWeapon(KDWeaponsTree &tree, std::vector<std::shared_ptr<Weapon>> &weapons) {
    std::shared_ptr<Weapon> temp = activeWeapon;
    activeWeapon = nullptr;

    pickWeapon(tree, weapons);

    if(temp != nullptr){
        weapons.push_back(temp);
        tree.addPoints(weapons.size() - 1, weapons.size() - 1);
    }
}

void Player::collisionCheck(Polygons &polygons, KDPolygonsTree &tree) {
    float query[2] = {position.x, position.y};
    float moveDistance = sqrt(pow(dx, 2) + pow(dy, 2));
    std::vector<std::pair<uint32_t, float>> ret_matches;
    tree.radiusSearch(query, 3 * pow(shapeRadius + moveDistance, 2), ret_matches, {});

    polygons.hideAll();
    for(auto &pair : ret_matches){
        polygons[pair.first].polygon.makeVisible();
    }

    std::vector<Edge> edges = polygons.collectEdges();
    bool edgeFound = false;
    Point collisionPoint{}, tempPoint{};
    float distToCollisionSqr = pow(viewDistance, 2);
    for(auto &edge : edges){
        if(unlimitedLineVSLineIntersection(position, Vector(dx, dy), edge.first, Vector(edge.first, edge.second),
                                           tempPoint) &&
           Vector(position, tempPoint).sqr() < distToCollisionSqr) {
            collisionPoint = tempPoint;
            edgeFound = true;
            distToCollisionSqr = Vector(position, tempPoint).sqr();
        }
    }

    if(edgeFound){
        float distToCollision = sqrt(Vector(position, collisionPoint).sqr());
        if(distToCollision - shapeRadius < shapeRadius + moveDistance){
            dx = (dx / (moveDistance + shapeRadius)) * (distToCollision - shapeRadius);
            dy = (dy / (moveDistance + shapeRadius)) * (distToCollision - shapeRadius);
        }
    }
}

void Player::updatePosition() {
    //set player coords:
    updateCoord(position.x, dx, screen_width);
    updateCoord(position.y, dy, screen_height);

    //update shape's position
    shape->setPosition(position.x, position.y);

    //update active weapon position
    if(activeWeapon != nullptr){
        activeWeapon->setPosition(position);
        activeWeapon->setAngle(angle);
    }
}

void Player::updateView(Polygons &polygons, const KDPolygonsTree &tree, const std::vector<std::shared_ptr<Entity>> &entities, std::list<std::shared_ptr<sf::Shape>> &viewShape
#ifdef T5_DEBUG
        , sf::RenderWindow &window
#endif
) {
    float query[2] = { position.x, position.y };
    std::vector<std::pair<uint32_t, float>> ret_matches;
    tree.radiusSearch(query, viewDistance * viewDistance, ret_matches, {});

    polygons.hideAll();
    for(auto &pair : ret_matches){
        polygons[pair.first].polygon.makeVisible();
    }

    polygons.updateVisibility(*this, entities, viewShape
#ifdef T5_DEBUG
            , window
#endif
    );
}

bool Player::takeDamage(float dealtDamage) {
    if(dealtDamage >= health){
        return true;
    }else{
        health -= dealtDamage;
        return false;
    }
}

void Player::updateCoord(float &coord, float &d, float limit) {
    coord += d;

    if(coord >= limit){
        coord = limit;
        d = 0;
    }
    else if(coord <= 0){
        coord = 0;
        d = 0;
    }
}

void Player::move(sf::Vector2i vector) {
    position += vector;
}

void Player::accelerate(float dX, float dY) {
    this->dx += dX * step;
    this->dy += dY * step;
}

unsigned int Player::getAmmo() const {
    return activeWeapon == nullptr ? 0 : activeWeapon->getAmmo();
}
