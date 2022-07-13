#include <iostream>
#include <fstream>

#include "Player.h"
#include "Polygon.h"
#include "Weapon.h"

Player::Player(std::vector<float> args) :
Entity(Point((float) screen_width / 2, (float) screen_height / 2)) ,
friction(args[0]) , step(args[1]) , viewDistance(args[2]) , shapeRadius(args[3]) , viewAngle(degToRad(args[4])) ,
maxHealth(args[5])
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
maxHealth(MAXHEALTH)
{
    health = maxHealth;

    auto playerShape = std::make_shared<sf::CircleShape>(shapeRadius);
    playerShape->setFillColor(defPlayerColor);
    playerShape->setPosition(position.x, position.y);
    playerShape->setOrigin(shapeRadius, shapeRadius);

    shape = playerShape;
}

void Player::update(Polygons &polygons, my_kd_tree_t &tree, const sf::Vector2i &sfMousePos, const std::vector<std::shared_ptr<Entity>> &entities,
                    std::list<std::shared_ptr<sf::Shape>> &viewShape
#ifdef T5_DEBUG
               , sf::RenderWindow &window
#endif
               ) {
    //check if mouse position == player position and if so - move mouse slightly up
    Point mousePos(sfMousePos);
    if(position == mousePos){
        mousePos.y -= 1;
    }

    //tree parameters
    nanoflann::SearchParams params;
    std::vector<std::pair<uint32_t, float>> ret_matches;

    //rotate view sector:
    angle = getAngleToZero(sf::Vector2f(mousePos.x - position.x, mousePos.y - position.y));

    //check for collision:
    float query[2] = {position.x, position.y};
    float moveDistance = sqrt(dx * dx + dy * dy);
    tree.radiusSearch(query, 3 * pow(shapeRadius + moveDistance, 2), ret_matches, params);

    polygons.hideAll();
    for(auto &pair : ret_matches){
        polygons[pair.first].polygon.makeVisible();
    }

    std::vector<Edge> edges = polygons.collectEdges();
    Edge nearestEdge;
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

    //set player coords:
    updateCoord(position.x, dx, screen_width);
    updateCoord(position.y, dy, screen_height);

    updateSpeed();

    //update shape's position
    shape->setPosition(position.x, position.y);

    //set forms' shapes:
    query[0] = position.x;  query[1] = position.y;
    tree.radiusSearch(query, viewDistance * viewDistance, ret_matches, params);

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

void Player::updateSpeed() {
    float d = sqrt(pow(dx, 2) + pow(dy, 2));
    float frictionX = dx * friction / d;
    float frictionY = dy * friction / d;
    dx = std::abs(dx) > frictionX ? dx - frictionX : 0;
    dy = std::abs(dy) > frictionY ? dy - frictionY : 0;

    d = sqrt(pow(dx, 2) + pow(dy, 2));
    if(d > SPEEDLIMIT){
        dx /= d / SPEEDLIMIT;
        dy /= d / SPEEDLIMIT;
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

bool Player::shoot(const std::vector<std::shared_ptr<Player>> &enemies, Point &where, std::shared_ptr<Player> &target){//true if target died
    if(activeWeapon == nullptr){
        return false;
    }

    std::vector<Point> targetPoints;
    target = nullptr;

    float distSqr = pow(viewDistance, 2);
    const Point *tp = nullptr;
    Point hitPoint(where);
    for(const auto &enemy : enemies){
        std::vector<Point> hitPoints = circleVSLineIntersection(position, where, enemy->getPosition(), enemy->getRadius());
        if(hitPoints.empty()){
            continue;
        }else if(hitPoints.size() == 2){
            if(Vector(position, hitPoints.front()).sqr() > Vector(position, hitPoints.back()).sqr()){
                tp = &hitPoints.back();
            }
            else{
                tp = &hitPoints.front();
            }
        }else{
            tp = &hitPoints.front();
        }

        float td = Vector(position, *tp).sqr();
        if(td < distSqr){
            distSqr = td;
            target = enemy;
            hitPoint = *tp;
        }
    }
    where = hitPoint;

    if(target == nullptr){
        if(vectorVSEdgesIntersectionCheck(position, Vector(position, where), blockingEdges)) {
            where = findNearestPoint(position, vectorVSEdgesIntersection(position, Vector(position, where), blockingEdges));
        }
        return false;
    }else {
        if(vectorVSEdgesIntersectionCheck(position, Vector(position, target->getPosition()), blockingEdges)){
            where = findNearestPoint(position, vectorVSEdgesIntersection(position, Vector(position, where), blockingEdges));

            return false;
        }else {
            return activeWeapon->shoot(target);
        }
    }
}
