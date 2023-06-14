#include <utility>
#include <cmath>
#include <cassert>

#include "Entity.h"
#include "Constants.h"
#include "JSONDataNode.h"

Entity::Entity(const DataNode &dataNode) :
    SpriteHolder(dataNode) , position(dataNode.getNumber(XKEY), dataNode.getNumber(YKEY)) ,
    bodyRadius(dataNode.getNumber(BODYRADIUSKEY))
{
    if(dataNode.contains(ANGLEKEY)){
        angle = dataNode.getNumber(ANGLEKEY);
    }
}

//Entity::Entity() : SpriteHolder("") , center(0, 0) {
//    assert(false && "Default constructor for Entity called");
//}

const Point &Entity::getPosition() const {
    return position;
}

double Entity::getAngle() const {
    return angle;
}

double Entity::getBodyRadius() const {
    return bodyRadius;
}

void Entity::setPosition(const Point &position_) {
    position = position_;
    setSpritePosition(position);
}

void Entity::setAngle(double angle_){
    while(angle_ >= 2 * PI_VALUE) angle_ -= 2 * PI_VALUE;
    angle = angle_;
    setSpriteAngle(angle);
}

std::string Entity::serialize() const {
    using Formatter = JSONDataNode;
    return Formatter::formatNumber(XKEY, position.x) + "," +
           Formatter::formatNumber(YKEY, position.y) + "," +
           Formatter::formatNumber(ANGLEKEY, angle) + "," +
           Formatter::formatNumber(BODYRADIUSKEY, bodyRadius);
}
