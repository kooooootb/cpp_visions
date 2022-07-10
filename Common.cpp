#include <SFML/Graphics.hpp>

#include <list>
#include <vector>
#include <memory>
#include <cmath>
#include <iostream>

#include "Common.h"

void drawAll(sf::RenderWindow &window, const std::vector<std::shared_ptr<sf::Shape>> &shapes){
	for(const auto &it : shapes){
		window.draw(*it);
	}
}
void drawAll(sf::RenderWindow &window, const std::list<std::shared_ptr<sf::Shape>> &shapes){
	for(const auto &it : shapes){
		window.draw(*it);
	}
}

void createBackground(std::vector<std::shared_ptr<sf::Shape>> &shapes){
	std::shared_ptr<sf::RectangleShape> backgroundShape = std::make_shared<sf::RectangleShape>(sf::Vector2f(screen_width, screen_height));
	backgroundShape->setPosition(0, 0);
//	backgroundShape->setFillColor(sf::Color(228, 228, 228));
	backgroundShape->setFillColor(sf::Color(10, 10, 10));
	shapes.push_back(backgroundShape);
}

int initViewSector(std::shared_ptr<sf::ConvexShape> &viewShape, const Point &center, float viewDistance, float currentAngle, float viewAngle){
    if(viewAngle > currentAngle){
        viewAngle -= 2 * M_PI;
    }
    viewAngle = currentAngle - viewAngle;

	int dotsAmount = (int) (viewDistance * viewAngle) - 2;//dots in arc

    if(dotsAmount < 1){
        return 2;
    }

    sf::Vector2f first = viewShape->getPoint(0);
    sf::Vector2f second = viewShape->getPoint(1);

	viewShape->setPointCount(dotsAmount + 3);//1 point reserved for center

    viewShape->setPoint(0, first);
    viewShape->setPoint(1, second);

	float dAngle = viewAngle / (float) (dotsAmount + 1);
    currentAngle -= dAngle;

    int index = 0;
	for(;index < dotsAmount;++index){
		viewShape->setPoint(index + 2, sf::Vector2f((float) (center.x + viewDistance * cos((double) currentAngle)),
												(float) (center.y + viewDistance * sin((double) currentAngle))));
		
		currentAngle -= dAngle;
	}

    return dotsAmount + 2;
}

Point Point::operator+(const Vector &vector) const {
    return { x + vector.x, y + vector.y };
}

Point Point::operator-(const Vector &vector) const {
    return { x - vector.x, y - vector.y };
}
