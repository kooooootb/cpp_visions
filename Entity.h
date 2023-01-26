#ifndef T5_ENTITY_H
#define T5_ENTITY_H

#include <SFML/Graphics.hpp>

#include "Common.h"

#include <memory>

class Entity {
private:
    void setSprite(const std::string &fname){
        defTexture.loadFromFile(texturesDirectory + fname);
        sprite->setTexture(defTexture);

        sprite->setOrigin(sprite->getLocalBounds().width / 2, sprite->getLocalBounds().height / 2);
        sprite->setPosition((float) position.x, (float) position.y);
    }

protected:
    Point position;
    double angle;
    std::shared_ptr<sf::Sprite> sprite = std::make_shared<sf::Sprite>();
    const sf::Color shapeColor = defPlayerColor;
    sf::Texture defTexture;

    bool visible = true;

    void setTexture(const sf::Texture &texture){
        sprite = std::make_shared<sf::Sprite>();
        sprite->setTexture(texture);

        sprite->setOrigin(sprite->getLocalBounds().width / 2, sprite->getLocalBounds().height / 2);
        sprite->setPosition((float) position.x, (float) position.y);
    }

public:
    Entity() = delete;
    Entity(const std::string &fname, const Point &point) : position(point) , angle(0) { setSprite(fname); }
    Entity(const std::string &fname, const Point &point, double Angle) : position(point) , angle(Angle) { setSprite(fname); }
    Entity(const std::string &fname, double X, double Y) : position(X, Y) , angle(0) { setSprite(fname); }

    double getX()const { return position.x; }
    double getY()const { return position.y; }
    double getAngle()const { return angle; }
    const Point &getPosition()const { return position; }
    const std::shared_ptr<sf::Sprite> &getShape() const { return sprite; }

    void setAngle(double ang) {
        angle = ang;

        sprite->setRotation((float) radToDeg(angle));
    }

    void setPosition(const Point &point){
        position = point;
        sprite->setPosition((float) point.x, (float) point.y);
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
