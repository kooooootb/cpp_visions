#include <utility>
#include <limits>

#include "Movable.h"
#include "Intersections.h"
#include "JSONDataNode.h"
#include "Utility.h"

Movable::Movable(const DataNode &dataNode) :
    Entity(dataNode) , friction(dataNode.getNumber(FRICTIONKEY)) ,
    maxSpeed(dataNode.getNumber(MAXSPEEDKEY)) , acceleration(dataNode.getNumber(ACCELERATIONKEY))
{}

void Movable::accelerate(const Vector &direction) {
    double coeff = acceleration / direction.length();
    velocity += direction * coeff;

    if(velocity.sqr() > std::pow(maxSpeed, 2)){
        posm_t v = velocity.length();
        velocity *= maxSpeed / v;
    }
}

bool Movable::move(const Polygon::Array &polygons) {
    double duration = getDuration();
    Vector moveBy = velocity * duration;

    bool hitSmth = adjustByCollision(polygons, moveBy);
    setPosition(getPosition() + moveBy);
    applyFriction(duration);

    return hitSmth;
}

bool Movable::adjustByCollision(const Polygon::Array &polygons, Vector &moveBy) {
    // how much will creature move
    double moveDistance = moveBy.length();
    // if moveDistance is too low don't move
    if(moveDistance <= 0){
        return false;
    }
    // how much space it needs
    double checkDistance = moveDistance + getBodyRadius();
    Vector checkVector(moveBy.x + getBodyRadius() * moveBy.x / moveDistance,
                       moveBy.y + getBodyRadius() * moveBy.y / moveDistance);

    Point topRight(checkVector.x > 0 ? getPosition().x + checkVector.x : getPosition().x,
                   checkVector.y > 0 ? getPosition().y + checkVector.y : getPosition().y); // bottom right on the screen
    Point bottomLeft(checkVector.x > 0 ? getPosition().x : getPosition().x + checkVector.x,
                     checkVector.y > 0 ? getPosition().y : getPosition().y + checkVector.y); // top left on the screen

    bool edgeFound = false;
    std::unique_ptr<Point> collisionPoint, tempPoint;
    double distToCollisionSqr = std::numeric_limits<double>::max();

    for(const auto &pol : *polygons){
        // find polygons that can intersect
        if(pol->bboxColliding(bottomLeft, topRight)){
            auto end = pol->getPoints().end();
            auto first = end - 1;
            auto second = pol->getPoints().begin();

            while(second != end){
                // find if they really intersect
                tempPoint = Inter::infLineVSLine(getPosition(), checkVector, *first, Vector(*first, *second));
                if(tempPoint != nullptr && Vector(getPosition(), *tempPoint).sqr() < distToCollisionSqr) {
                    collisionPoint = std::move(tempPoint);
                    edgeFound = true;
                    distToCollisionSqr = Vector(getPosition(), *collisionPoint).sqr();
                }

                first = second;
                ++second;
            }
        }
    }

    if(edgeFound){
        double distToCollision = std::sqrt(distToCollisionSqr);

        if(distToCollision - getBodyRadius() < getBodyRadius() + moveDistance) {
            moveBy.x = moveBy.x * (distToCollision - getBodyRadius()) / checkDistance;
            moveBy.y = moveBy.y * (distToCollision - getBodyRadius()) / checkDistance;
        }
    }

    return edgeFound;
}

void Movable::applyFriction(double duration) {
    posm_t v = velocity.length();

    if(Util::almostEqual(v, 0.)){
        velocity.x = 0;
        velocity.y = 0;
    } else{
        double applying = friction * duration;

        double applyingX = -velocity.x * applying / v;
        double applyingY = -velocity.y * applying / v;

        bool sign;

        sign = velocity.x > 0;
        if((velocity.x + applyingX > 0) == sign) velocity.x += applyingX;
        else velocity.x = 0;

        sign = velocity.y > 0;
        if((velocity.y + applyingY > 0) == sign) velocity.y += applyingY;
        else velocity.y = 0;
    }
}

std::string Movable::serialize() const {
    using Formatter = JSONDataNode;
    return Formatter::formatNumber(FRICTIONKEY, friction) + "," +
           Formatter::formatNumber(MAXSPEEDKEY, maxSpeed);
}

const Vector &Movable::getVelocity() const {
    return velocity;
}

posm_t Movable::getMaxSpeed() const {
    return maxSpeed;
}
