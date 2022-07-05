#include <SFML/Graphics.hpp>

#include <list>
#include <vector>
#include <memory>
#include <cmath>

#include "Common.h"

void drawAll(sf::RenderWindow &window, const std::vector<std::shared_ptr<sf::Shape>> &shapes){
	for(const auto &it : shapes){
		window.draw(*it);
	}
}

void createBackground(std::vector<std::shared_ptr<sf::Shape>> &shapes){
	std::shared_ptr<sf::RectangleShape> backgroundShape = std::make_shared<sf::RectangleShape>(sf::Vector2f(screen_width, screen_height));
	backgroundShape->setPosition(0, 0);
	backgroundShape->setFillColor(sf::Color(228, 228, 228));
	shapes.push_back(backgroundShape);
}

void initViewSector(std::shared_ptr<sf::ConvexShape> &viewShape, float viewDistance, float viewAngle){
	int dotsAmount = (int) (viewDistance * viewAngle);//dots in arc
	
	int index = 0;
	viewShape->setPointCount(dotsAmount + 1);
	float dAngle = viewAngle / (float) (dotsAmount - 1);
	float currentAngle = M_PI_2 + viewAngle / 2.0;
	
	for(;index < dotsAmount;++index){
		viewShape->setPoint(index, sf::Vector2f((float) (viewDistance * cos((double) currentAngle)),
												(float) (-1 * viewDistance * sin((double) currentAngle))));
		
		currentAngle = currentAngle - dAngle;
	}
	
	viewShape->setPoint(index, sf::Vector2f(0, 0));
}