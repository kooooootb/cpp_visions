#include <SFML/Graphics.hpp>

#include <list>
#include <vector>
#include <memory>

#include "Common.h"

void createBackground(std::vector<std::shared_ptr<sf::Shape>> &shapes){
    std::shared_ptr<sf::RectangleShape> backgroundShape = std::make_shared<sf::RectangleShape>(sf::Vector2f(screen_width, screen_height));

    backgroundShape->setPosition(0, 0);
	backgroundShape->setFillColor(sf::Color(228, 228, 228));
//    backgroundShape->setFillColor(sf::Color(10, 10, 10));

    shapes.push_back(backgroundShape);
}

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

void drawLines(sf::RenderWindow &window, const sf::VertexArray &lines){
	window.draw(lines);
}

int main(){
	sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "15");
	window.setFramerateLimit(60);
	
	std::vector<std::shared_ptr<sf::Shape>> shapes;
	sf::VertexArray lines(sf::LineStrip);
	std::vector<std::shared_ptr<std::list<sf::Vector2f>>> polygons;
	
	createBackground(shapes);
	
	std::shared_ptr<std::list<sf::Vector2f>> curCoords = std::make_shared<std::list<sf::Vector2f>>();
	
	loadLevel(shapes, levelFname, polygons);
	
	sf::Event event{};
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			switch (event.type){
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::MouseButtonPressed: {
					//get next point
					curCoords->emplace_back(sf::Mouse::getPosition(window));
					
					//draw point
					std::shared_ptr<sf::CircleShape> point = std::make_shared<sf::CircleShape>(point_radius);
					point->setFillColor(sf::Color::Black);
					point->setOrigin(point_radius, point_radius);
					point->setPosition(curCoords->back());
					shapes.push_back(point);
					
					//draw line
					sf::Vertex vertex(curCoords->back(), sf::Color::Black);
					lines.append(vertex);
					
					break;
				}
				case sf::Event::KeyPressed:
					switch(event.key.code){
						case sf::Keyboard::E:
							if(curCoords->size() > 2){
								sf::ConvexShape convex;
								convex.setPointCount(curCoords->size());
								convex.setFillColor(sf::Color::Black);
								int i = 0;
								for(const auto &it : *curCoords){
									convex.setPoint(i++, it);
								}
								shapes.push_back(std::make_shared<sf::ConvexShape>(convex));
								
								polygons.emplace_back(curCoords);
								curCoords = std::make_shared<std::list<sf::Vector2f>>();
								
								lines.clear();
							}
							
							break;
						case sf::Keyboard::O:
							shapes.clear();
							lines.clear();
							polygons.clear();
							createBackground(shapes);
							break;
						case sf::Keyboard::Escape:
							window.close();
							break;
						default:
							//default
							break;
					}
					break;
				default:
					//default
					break;
			}
		}
		
		window.clear();
		
		//draw
		drawAll(window, shapes);
		drawLines(window, lines);
		
		window.display();
	}
	
	//save in file
	saveLevel(levelFname, polygons);
	
	return 0;
}