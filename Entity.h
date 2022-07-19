#ifndef T5_ENTITY_H
#define T5_ENTITY_H

#include "Common.h"

#include <memory>

class Entity {
private:
    void setSprite(const std::string &fname){
        defTexture.loadFromFile(texturesDirectory + fname);
        sprite->setTexture(defTexture);

        sprite->setOrigin(sprite->getLocalBounds().width / 2, sprite->getLocalBounds().height / 2);
        sprite->setPosition(position.x, position.y);
    }

protected:
    Point position = {0, 0};
    float angle = 0;
    std::shared_ptr<sf::Sprite> sprite = std::make_shared<sf::Sprite>();
    const sf::Color shapeColor = defPlayerColor;
    sf::Texture defTexture;

    bool visible = true;

    void setTexture(const sf::Texture &texture){
        sprite = std::make_shared<sf::Sprite>();
        sprite->setTexture(texture);

        sprite->setOrigin(sprite->getLocalBounds().width / 2, sprite->getLocalBounds().height / 2);
        sprite->setPosition(position.x, position.y);
    }

public:
    Entity() = default;
    Entity(const std::string &fname,const Point &point) : position(point) { setSprite(fname); }
    Entity(const std::string &fname,const Point &point, float Angle) : position(point) , angle(Angle) { setSprite(fname); }
    Entity(const std::string &fname,float X, float Y) : position(X, Y) { setSprite(fname); }

    float getX()const { return position.x; }
    float getY()const { return position.y; }
    float getAngle()const { return angle; }
    const Point &getPosition()const { return position; }
    const std::shared_ptr<sf::Sprite> &getShape() const { return sprite; }

    void setAngle(float ang) {
        angle = ang;

        sprite->setRotation(radToDeg(angle));
    }

    void setPosition(const Point &point){
        position = point;
        sprite->setPosition(point.x, point.y);
    }

    void setVisibility(bool vis){
        visible = vis;
    }

    void draw(sf::RenderWindow &window){
        if(visible){
            window.draw(*sprite);
        }
    }
};


#endif //T5_ENTITY_H
