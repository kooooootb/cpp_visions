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

    float getX()const { return position.x; }
    float getY()const { return position.y; }
    const Point &getPosition()const { return position; }
    const std::shared_ptr<sf::Shape> &getShape() const { return shape; }
};


#endif //T5_ENTITY_H
