#include <iostream>
#include <fstream>
#include <exception>
#include <cassert>

#include "Common.h"
#include "Player.h"
#include "Polygon.h"
#include "Weapon.h"
#include "Projectile.h"

BasePlayer::BasePlayer(const std::string &name, double x_, double y_, double friction_, double step_,
                       double viewDistance_, double shapeRadius_, double viewAngleDeg_, int maxHealth_, double maxSpeed_) :
        Entity(name + "_unarmed.png", x_, y_) , friction(friction_) , step(step_) ,
        viewDistance(viewDistance_) , shapeRadius(shapeRadius_) , viewAngle(degToRad(viewAngleDeg_)) ,
        maxHealth(maxHealth_) , maxSpeed(maxSpeed_)
{
    health = maxHealth;

    std::string fileName;
    for(const auto &gunName : availableWeapons){
        fileName = "";
        armedTextures[gunName].loadFromFile(((((fileName += texturesDirectory) += name) += "_") += gunName) += ".png");
    }
}

Player::Player(const std::string &name_, double x_, double y_, double friction_, double step_, double viewDistance_,
                       double shapeRadius_, double viewAngleDeg_, int maxHealth_, double maxSpeed_) :
        BasePlayer(name_, x_, y_, friction_,  step_,  viewDistance_, shapeRadius_,
                   viewAngleDeg_, maxHealth_,  maxSpeed_) {

}

Enemy::Enemy(const std::string &name_, double x_, double y_, double friction_, double step_, double viewDistance_,
                       double shapeRadius_, double viewAngleDeg_, int maxHealth_, double maxSpeed_) :
        BasePlayer(name_, x_, y_, friction_,  step_,  viewDistance_, shapeRadius_,
                   viewAngleDeg_, maxHealth_,  maxSpeed_) {

}

Player Player::loadPlayer(const std::string &fname) {
    std::ifstream fd(fname);
    assert(fd.is_open());

    double x, y, friction, step, viewDistance, shapeRadius, viewAngleDeg, maxSpeed;
    int maxHealth;

    fd.read(reinterpret_cast<char *>(&x), sizeof(x));
    fd.read(reinterpret_cast<char *>(&y), sizeof(y));
    fd.read(reinterpret_cast<char *>(&friction), sizeof(friction));
    fd.read(reinterpret_cast<char *>(&step), sizeof(step));
    fd.read(reinterpret_cast<char *>(&viewDistance), sizeof(viewDistance));
    fd.read(reinterpret_cast<char *>(&shapeRadius), sizeof(shapeRadius));
    fd.read(reinterpret_cast<char *>(&viewAngleDeg), sizeof(viewAngleDeg));
    fd.read(reinterpret_cast<char *>(&maxHealth), sizeof(maxHealth));
    fd.read(reinterpret_cast<char *>(&maxSpeed), sizeof(maxSpeed));

    std::string name = readFname(fd);

    fd.close();

    return { name, x, y, friction, step, viewDistance, shapeRadius, viewAngleDeg, maxHealth, maxSpeed };
}

Enemy Enemy::loadEnemy(const std::string &fname) {
    std::ifstream fd(fname);
    assert(fd.is_open());

    double x, y, friction, step, viewDistance, shapeRadius, viewAngleDeg, maxSpeed;
    int maxHealth;

    fd.read(reinterpret_cast<char *>(&x), sizeof(x));
    fd.read(reinterpret_cast<char *>(&y), sizeof(y));
    fd.read(reinterpret_cast<char *>(&friction), sizeof(friction));
    fd.read(reinterpret_cast<char *>(&step), sizeof(step));
    fd.read(reinterpret_cast<char *>(&viewDistance), sizeof(viewDistance));
    fd.read(reinterpret_cast<char *>(&shapeRadius), sizeof(shapeRadius));
    fd.read(reinterpret_cast<char *>(&viewAngleDeg), sizeof(viewAngleDeg));
    fd.read(reinterpret_cast<char *>(&maxHealth), sizeof(maxHealth));
    fd.read(reinterpret_cast<char *>(&maxSpeed), sizeof(maxSpeed));

    std::string name = readFname(fd);

    fd.close();

    return { name, x, y, friction, step, viewDistance, shapeRadius, viewAngleDeg, maxHealth, maxSpeed };
}

void Player::update(const std::vector<std::shared_ptr<Polygon>> &polygons, const sf::Vector2f &mousePos,
                    const std::vector<std::shared_ptr<Entity>> &entities) {
    //rotate view sector:
    setAngle(getAngleToZero(Vector(mousePos.x - position.x, mousePos.y - position.y)));

    //update speed
    updateSpeed();

    //update positions
    updatePosition(polygons);

    //set forms' shapes:
    updateView(polygons, entities);

    //make player visible
    setVisibility(true);
}

void BasePlayer::updateSpeed() {
    double d = sqrt(pow(dx, 2) + pow(dy, 2));
    double frictionX = dx * friction / d;
    double frictionY = dy * friction / d;
    dx = std::abs(dx) > frictionX ? dx - frictionX : 0;
    dy = std::abs(dy) > frictionY ? dy - frictionY : 0;

    d = sqrt(pow(dx, 2) + pow(dy, 2));
    if(d > maxSpeed){
        dx /= d / maxSpeed;
        dy /= d / maxSpeed;
    }
}

bool BasePlayer::shoot(std::vector<std::shared_ptr<BasePlayer>> &players, std::shared_ptr<Projectile> &projectile, const std::vector<std::shared_ptr<Polygon>> &polygons){//true if able to shoot
    if(activeWeapon == nullptr || activeWeapon->spendBullet()){
        return false;
    }

    projectile = std::make_shared<Projectile>(*activeWeapon, players, polygons);

    return true;
}

void BasePlayer::pickWeapon(std::vector<std::shared_ptr<Weapon>> &weapons) {
    if(!activeWeapon){//active weapon should be clear anyway
        std::shared_ptr<Weapon> foundWeapon = nullptr;
        for(const auto &weapon : weapons){
            if(Vector(weapon->getPosition(), position).sqr() < PICKUPRADIUSSQR){
                foundWeapon = weapon;
                break;
            }
        }

        if(foundWeapon){
            activeWeapon = foundWeapon;
            activeWeapon->setVisibility(false);
        }
    }
}

void BasePlayer::changeWeapon(std::vector<std::shared_ptr<Weapon>> &weapons) {
    std::shared_ptr<Weapon> temp = activeWeapon;
    activeWeapon = nullptr;

    pickWeapon(weapons);

    if(temp != nullptr){
        weapons.push_back(temp);
    }

    if(activeWeapon != nullptr){
        setTexture(armedTextures[activeWeapon->getName()]);
    } else if(temp != activeWeapon){
        setTexture(defTexture);
    }
}

void BasePlayer::collisionCheck(const std::vector<std::shared_ptr<Polygon>> &polygons) {
    double moveDistance = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
    double checkDistance = moveDistance + shapeRadius;
    Vector moveVector(dx + shapeRadius * dx / moveDistance, dy  + shapeRadius * dy / moveDistance);

    Point maxPoint = position + Vector(checkDistance, checkDistance);
    Point minPoint = position - Vector(checkDistance, checkDistance);
    bool edgeFound = false;
    Point collisionPoint, tempPoint;
    double distToCollisionSqr = std::numeric_limits<double>::max();

    for(const auto &pol : polygons){
        // find polygons that can intersect
        if(pol->minPoint().x < maxPoint.x && pol->minPoint().y < maxPoint.y && pol->maxPoint().x > minPoint.x && pol->maxPoint().y > minPoint.y){
            auto end = pol->getPoints().end();
            auto first = end - 1;
            auto second = pol->getPoints().begin();

            while(second != end){
                // find if they really intersect
                if(unlimitedLineVSLineIntersection(position, moveVector, *first, Vector(*first, *second), tempPoint) &&
                   Vector(position, tempPoint).sqr() < distToCollisionSqr) {
                    collisionPoint = tempPoint;
                    edgeFound = true;
                    distToCollisionSqr = Vector(position, collisionPoint).sqr();
                }

                first = second;
                ++second;
            }
        }
    }

    if(edgeFound){
        double distToCollision = std::sqrt(distToCollisionSqr);

        dx = dx * (distToCollision - shapeRadius) / checkDistance;
        dy = dy * (distToCollision - shapeRadius) / checkDistance;
    }
}

void BasePlayer::updatePosition(const std::vector<std::shared_ptr<Polygon>> &polygons) {
    //check for collision:
    collisionCheck(polygons);

    //set player coords:
    updateCoord(position.x, dx);
    updateCoord(position.y, dy);

    //update shape's position
    sprite->setPosition((float) position.x, (float) position.y);

    //update active weapon position
    if(activeWeapon != nullptr){
        activeWeapon->setPosition(position);
        activeWeapon->setAngle(angle);
    }
}

PointType Player::getPointType(const Point &point, const std::array<Vector, 3> &views)const{
    Vector pointVector(position, point);

    if (views[1] * pointVector < 0) {
        return behind;
    } else if (pointVector.sqr() > pow(viewDistance, 2)) {
        return outside;
    } else if (isOutOfBoarders(pointVector, views)) {
        return semicircle;
    } else {
        return within;
    }
}

void Player::updateView(const std::vector<std::shared_ptr<Polygon>> &polygons, const std::vector<std::shared_ptr<Entity>> &entities) {
    std::array<Vector, 3> views{{{(viewDistance * cos(angle + viewAngle)), (viewDistance * sin(angle + viewAngle))},
                                 {(viewDistance * cos(angle)), (viewDistance * sin(angle))},
                                 {(viewDistance * cos(angle - viewAngle)), (viewDistance * sin(angle - viewAngle))}}};

    //result arrays:
    blockingEdges.clear();
    std::list<Point> arcPoints, edgePoints/*TODO: why edgepoints*/, endPoints;

    //points for bbox intersection
    Point maxPoint = position + Vector(viewDistance, viewDistance);
    Point minPoint = position - Vector(viewDistance, viewDistance);

    //set points and blocking edges array
    for (const auto &pol: polygons) {
        //check if polygon not far away
        if (maxPoint.x > pol->minPoint().x && maxPoint.y > pol->minPoint().y && minPoint.x < pol->maxPoint().x && minPoint.y < pol->maxPoint().y) {
            auto endIt = pol->getPoints().end();
            auto previousIt = endIt - 1;
            auto currentIt = pol->getPoints().begin();
            PointType previousType = getPointType(*previousIt, views), currentType;

            while (currentIt != endIt) {
                currentType = getPointType(*currentIt, views);
                PointType currentTypeTemp = currentType;

                const Point *currentPointPtr = &*currentIt;
                const Point *previousPointPtr = &*previousIt;

                if (previousType > currentType) {
                    std::swap(currentPointPtr, previousPointPtr);
                    std::swap(currentType, previousType);
                }

                const Point &previousPoint = *previousPointPtr;
                const Point &currentPoint = *currentPointPtr;

                switch (previousType) {
                    case within:
                        switch (currentType) {
                            case within: {
                                //within - within
                                endPoints.push_back(previousPoint);
                                endPoints.push_back(currentPoint);
                                blockingEdges.emplace_back(previousPoint, currentPoint);
                                break;
                            }
                            case behind: {
                                //within - behind
                                endPoints.push_back(previousPoint);
                                std::vector<Point> intersectPoint =
                                        twoLinesVSLineIntersection(previousPoint, currentPoint,position, views);
                                if(!intersectPoint.empty()){
                                    edgePoints.push_back(intersectPoint.front());
                                }else{
                                    edgePoints.push_back(position);
                                }
                                blockingEdges.emplace_back(previousPoint, currentPoint);
                                break;
                            }
                            case semicircle: {
                                //within - semicircle
                                endPoints.push_back(previousPoint);
                                std::vector<Point> intersectPoint =
                                        twoLinesVSLineIntersection(previousPoint, currentPoint, position, views);
                                edgePoints.push_back(intersectPoint.front());
                                blockingEdges.emplace_back(previousPoint, currentPoint);
                                break;
                            }
                            case outside: {
                                //within - outside
                                endPoints.push_back(previousPoint);
                                std::vector<Point> intersectPoint =
                                        arcSegmentVSLineIntersection(previousPoint, currentPoint, position, viewDistance, views);
                                if (intersectPoint.empty()) {
                                    intersectPoint = twoLinesVSLineIntersection(previousPoint, currentPoint, position, views);
                                    edgePoints.push_back(intersectPoint.front());
                                } else {
                                    arcPoints.push_back(intersectPoint.front());
                                }
                                blockingEdges.emplace_back(previousPoint, currentPoint);
                                break;
                            }
                        }
                        break;
                    case behind:
                        switch (currentType) {
                            case behind: {
                                //behind - behind
                                //prune
                                break;
                            }
                            case outside: {
                                //behind - outside
                                std::vector<Point> intersectPoint = twoLinesVSLineIntersection(previousPoint, currentPoint,
                                                                                               position, views);
                                if (intersectPoint.empty()) {
                                    intersectPoint =
                                            arcSegmentVSLineIntersection(previousPoint, currentPoint, position, viewDistance, views);
                                    if (intersectPoint.size() == 2) {
                                        arcPoints.push_back(intersectPoint.front());
                                        arcPoints.push_back(intersectPoint.back());
                                        blockingEdges.emplace_back(previousPoint, currentPoint);
                                    }
                                } else if (intersectPoint.size() == 1) {
                                    edgePoints.push_back(intersectPoint.front());
                                    intersectPoint =
                                            arcSegmentVSLineIntersection(previousPoint, currentPoint, position, viewDistance, views);
                                    arcPoints.push_back(intersectPoint.front());
                                    blockingEdges.emplace_back(previousPoint, currentPoint);
                                } else {
                                    edgePoints.push_back(intersectPoint.front());
                                    edgePoints.push_back(intersectPoint.back());
                                    blockingEdges.emplace_back(previousPoint, currentPoint);
                                }
                                break;
                            }
                            case semicircle: {
                                //behind - semicircle
                                std::vector<Point> intersectPoint =
                                        twoLinesVSLineIntersection(previousPoint, currentPoint, position, views);
                                if (!intersectPoint.empty()) {
                                    edgePoints.push_back(intersectPoint.front());
                                    edgePoints.push_back(intersectPoint.back());
                                    blockingEdges.emplace_back(previousPoint, currentPoint);
                                }
                                break;
                            }
                            default:
                                assert(false);
                        }
                        break;
                    case semicircle:
                        switch (currentType) {
                            case semicircle: {
                                //semicircle - semicircle
                                std::vector<Point> intersectPoint =
                                        twoLinesVSLineIntersection(previousPoint, currentPoint, position, views);
                                if (!intersectPoint.empty()) {
                                    edgePoints.push_back(intersectPoint.front());
                                    edgePoints.push_back(intersectPoint.back());
                                    blockingEdges.emplace_back(previousPoint, currentPoint);
                                }
                                break;
                            }
                            case outside: {
                                //semicircle - outside
                                std::vector<Point> intersectPoint =
                                        twoLinesVSLineIntersection(previousPoint, currentPoint, position, views);
                                if (intersectPoint.size() == 2) {
                                    edgePoints.push_back(intersectPoint.front());
                                    edgePoints.push_back(intersectPoint.back());
                                    blockingEdges.emplace_back(previousPoint, currentPoint);
                                } else if (intersectPoint.size() == 1) {
                                    edgePoints.push_back(intersectPoint.front());
                                    intersectPoint =
                                            arcSegmentVSLineIntersection(previousPoint, currentPoint, position,
                                                                         viewDistance, views);
                                    arcPoints.push_back(intersectPoint.front());
                                    blockingEdges.emplace_back(previousPoint, currentPoint);
                                }
                                break;
                            }
                            default:
                                assert(false);
                        }
                        break;
                    case outside: {
                        //outside - outside
                        std::vector<Point> intersectPoint =
                                twoLinesVSLineIntersection(previousPoint, currentPoint, position, views);
                        if (intersectPoint.empty()) {//2 on arc
                            intersectPoint =
                                    arcSegmentVSLineIntersection(previousPoint, currentPoint, position, viewDistance, views);
                            if (intersectPoint.size() == 2) {
                                arcPoints.push_back(intersectPoint.front());
                                arcPoints.push_back(intersectPoint.back());
                                blockingEdges.emplace_back(previousPoint, currentPoint);
                            }
                        } else if (intersectPoint.size() == 1) {//1 on edge, 1 on arc
                            edgePoints.push_back(intersectPoint.front());
                            intersectPoint =
                                    arcSegmentVSLineIntersection(previousPoint, currentPoint, position, viewDistance, views);
                            if (intersectPoint.size() == 1) {
                                arcPoints.push_back(intersectPoint.front());
                                blockingEdges.emplace_back(previousPoint, currentPoint);
                            }
                        } else {//2 on edges
                            edgePoints.push_back(intersectPoint.front());
                            edgePoints.push_back(intersectPoint.back());
                            blockingEdges.emplace_back(previousPoint, currentPoint);
                        }
                        break;
                    }
                }

                previousType = currentTypeTemp;
                previousIt = currentIt;
                ++currentIt;
            }
        }
    }

    // resulting point array
    std::vector<Point> hitPoints;
    hitPoints.reserve(2 + arcPoints.size() + endPoints.size());

    //generate resulting points using ray check and collect them into array
    for (int i = 0; i < 2; ++i) {
        Vector pointVector(position, position + views[2 * i]);
        hitPoints.push_back(rayCheck(position, viewDistance, pointVector, blockingEdges));
    }

    for (const auto &point: arcPoints) {
        Vector pointVector(position, point);
        hitPoints.push_back(rayCheck(position, viewDistance, pointVector, blockingEdges));
    }

    for (const auto &point: endPoints) {//create 2 auxiliary points
        Vector pointVector(position, point);
        hitPoints.push_back(rayCheck(position, viewDistance, pointVector, blockingEdges));

        pointVector.rotate(degToRad(1));
        if (!isOutOfBoarders(pointVector, views)) {
            hitPoints.push_back(rayCheck(position, viewDistance, pointVector, blockingEdges));
        }

        pointVector.rotate(degToRad(-2));
        if (!isOutOfBoarders(pointVector, views)) {
            hitPoints.push_back(rayCheck(position, viewDistance, pointVector, blockingEdges));
        }
    }

    //clear space from used points
    arcPoints.clear();
    endPoints.clear();

    //sort hitPoints by angle
    std::sort(hitPoints.begin(), hitPoints.end(), [this](const Point &p1, const Point &p2) -> bool {
        Vector v1(position, p1);
        Vector v2(position, p2);

        return v1.cross(v2) > 0;
    });

    //get rid of points at the same angle from center (store othrer points in different array)
    std::vector<Point> resPoints;
    for (auto it = hitPoints.begin(), prevIt = hitPoints.end(), endIt = hitPoints.end(); it < endIt; ++it) {
        Vector itVector(position, *it);

        if (prevIt == hitPoints.end() || std::abs(itVector.cross(Vector(position, *prevIt))) > epsilon) {
            resPoints.push_back(*it);
        }

        prevIt = it;
    }

    //clear useless array
    hitPoints.clear();

    //set visible enemies
    for (const auto &entity : entities) {
        bool visible = false;
        Vector enemyVector(position, entity->getPosition());

        if (enemyVector.sqr() < viewDistance * viewDistance && views[1] * enemyVector > 0 && !isOutOfBoarders(enemyVector, views)) {
            if(!vectorVSEdgesIntersectionCheck(position, enemyVector, blockingEdges)){
                visible = true;
            }
        }

        entity->setVisibility(visible);
    }

    //set convex
    viewShape.clear();
    bool prevLiesOnArc = std::abs(Vector(position, resPoints.front()).sqr() - viewDistance * viewDistance) < epsilon;
    bool usedArc;

    for(auto curIt = resPoints.begin() + 1, prevIt = resPoints.begin(), endIt = resPoints.end(); curIt < endIt; ++curIt, ++prevIt){
        usedArc = false;
        bool itLiesOnArc = std::abs(Vector(position, *curIt).sqr() - viewDistance * viewDistance) < epsilon;

        if(prevLiesOnArc && itLiesOnArc){
            Edge closestEdge;
            bool edgeFound = findClosestEdge(blockingEdges, closestEdge, *curIt);

            if(edgeFound && std::abs(Vector(closestEdge.first, closestEdge.second).cross(Vector(*curIt, *prevIt))) > epsilon || !edgeFound) {
                Vector prevVector(position, *prevIt);
                Vector itVector(position, *curIt);
                Vector zeroVector(1, 0);

                double prevAngle = prevVector.y > 0 ?
                        acos((zeroVector * prevVector) / viewDistance) : 2 * M_PI - acos((zeroVector * prevVector) / viewDistance);
                double itAngle = itVector.y > 0 ?
                        acos((zeroVector * itVector) / viewDistance) : 2 * M_PI - acos((zeroVector * itVector) / viewDistance);

                initViewSector(viewShape, position, viewDistance, itAngle, prevAngle);
                usedArc = true;
            }
        }

        if(!usedArc){
            size_t index = viewShape.getVertexCount();
            viewShape.resize(index + 2);

            viewShape[index + 1].position = sf::Vector2f((float) prevIt->x, (float) prevIt->y);
            viewShape[index + 2].position = sf::Vector2f((float) curIt->x, (float) curIt->y);
        }
        prevLiesOnArc = itLiesOnArc;
    }
}

bool BasePlayer::takeDamage(unsigned int dealtDamage) {
    if(dealtDamage >= health){
        return true;
    }else{
        health -= dealtDamage;
        return false;
    }
}

void BasePlayer::updateCoord(double &coord, double &d) {
    coord += d;
}

void BasePlayer::move(sf::Vector2f vector) {
    position += vector;
}

void BasePlayer::accelerate(float dX, float dY) {
    this->dx += dX * step;
    this->dy += dY * step;
}

unsigned int BasePlayer::getAmmo() const {
    return activeWeapon == nullptr ? 0 : activeWeapon->getAmmo();
}

void Enemy::update(const std::vector<std::shared_ptr<Polygon>> &polygons, const sf::Vector2f &mousePos,
                   const std::vector<std::shared_ptr<Entity>> &entities) {

}
