#ifndef T5_ENTITY_H
#define T5_ENTITY_H

#include "Common.h"

#include <memory>

class Entity {
protected:
    Point position = {0, 0};
    float angle = 0;
    std::shared_ptr<sf::Shape> shape;
    const sf::Color shapeColor = defPlayerColor;

public:
    Entity() = default;
    Entity(const Point &point) : position(point) {}
    Entity(const Point &point, float Angle) : position(point) , angle(Angle) {}
    Entity(float X, float Y) : position(X, Y) {}

    float getX()const { return position.x; }
    float getY()const { return position.y; }
    float getAngle()const { return angle; }
    const Point &getPosition()const { return position; }
    const std::shared_ptr<sf::Shape> &getShape() const { return shape; }

    void setAngle(float ang) {
        angle = ang;
    }

    void setPosition(const Point &point){
        position = point;
        shape->setPosition(point.x, point.y);
    }

    void setVisibility(bool visible){
        if(visible){
            shape->setFillColor(shapeColor);
        }else{
            shape->setFillColor(sf::Color::Transparent);
        }
    }
};


#endif //T5_ENTITY_H
