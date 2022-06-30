#include <SFML/Graphics.hpp>

#include <list>
#include <vector>
#include <memory>

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