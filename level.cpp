#include <SFML/Graphics.hpp>

#include <iostream>
#include <list>
#include <vector>
#include <memory>

#include "Common.h"

void createBackground(std::vector<std::shared_ptr<sf::Shape>> &shapes, const std::pair<int, int> &boarders){
    std::shared_ptr<sf::RectangleShape> backgroundShape = std::make_shared<sf::RectangleShape>(sf::Vector2f(boarders.first + 200, boarders.second + 200));
    backgroundShape->setPosition(-100, -100);

	backgroundShape->setFillColor(sf::Color(228, 228, 228));

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

void moveView(sf::RenderWindow &window, const Vector &vector){
    sf::View view = window.getView();
    view.move(vector.x, vector.y);
    window.setView(view);
}

int main(){
	sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "15");
	window.setFramerateLimit(60);
	
	std::vector<std::shared_ptr<sf::Shape>> shapes;
	sf::VertexArray lines(sf::LineStrip);
	std::vector<std::shared_ptr<std::list<sf::Vector2f>>> polygons;
    std::vector<Point> weaponSps;
	
	std::shared_ptr<std::list<sf::Vector2f>> curCoords = std::make_shared<std::list<sf::Vector2f>>();
    std::pair<int, int> boarders = loadBoarders(levelFname);

    createBackground(shapes, boarders);

    auto setConvex = [&](std::shared_ptr<std::list<sf::Vector2f>> &coords){
        sf::ConvexShape convex;
        convex.setPointCount(coords->size());
        convex.setFillColor(sf::Color::Black);
        int i = 0;
        for(const auto &it : *coords){
            convex.setPoint(i++, it);
        }
        shapes.push_back(std::make_shared<sf::ConvexShape>(convex));
        polygons.emplace_back(coords);

        coords = std::make_shared<std::list<sf::Vector2f>>();
    };

    auto makeBoarders = [&]() {
        int d = 10;

        std::shared_ptr<std::list<sf::Vector2f>> coords = std::make_shared<std::list<sf::Vector2f>>();
        coords->emplace_back(-d, -d);
        coords->emplace_back(0, -d);
        coords->emplace_back(0, boarders.second + d);
        coords->emplace_back(-d, boarders.second + d);
        setConvex(coords);

        coords = std::make_shared<std::list<sf::Vector2f>>();
        coords->emplace_back(-d, 0);
        coords->emplace_back(-d, -d);
        coords->emplace_back(boarders.first, -d);
        coords->emplace_back(boarders.first, 0);
        setConvex(coords);

        coords = std::make_shared<std::list<sf::Vector2f>>();
        coords->emplace_back(boarders.first, -d);
        coords->emplace_back(boarders.first + d, -d);
        coords->emplace_back(boarders.first + d, boarders.second + d);
        coords->emplace_back(boarders.first, boarders.second + d);
        setConvex(coords);

        coords = std::make_shared<std::list<sf::Vector2f>>();
        coords->emplace_back(boarders.first + d, boarders.second);
        coords->emplace_back(boarders.first + d, boarders.second + d);
        coords->emplace_back(-d, boarders.second + d);
        coords->emplace_back(-d, boarders.second);
        setConvex(coords);
    };

    if(!loadLevel(shapes, levelFname, polygons, weaponSps)){
        std::cout << "Enter level width:";
        std::cin >> boarders.first;
        std::cout << "Enter level height:";
        std::cin >> boarders.second;

        makeBoarders();
    }

    sf::View view(sf::FloatRect(0, 0, screen_width, screen_height));
    window.setView(view);

	sf::Event event{};
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			switch (event.type){
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::MouseButtonPressed: {
					//get next point
					curCoords->emplace_back(getMousePosition(window));
					
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
								setConvex(curCoords);
								
								lines.clear();
							}
							
							break;
						case sf::Keyboard::O:
							shapes.clear();
                            weaponSps.clear();
							lines.clear();
							polygons.clear();
							createBackground(shapes, boarders);
                            makeBoarders();
							break;
                        case sf::Keyboard::Q: {
                            sf::Vector2f sfMousePos = getMousePosition(window);
                            Point mousePos(sfMousePos.x, sfMousePos.y);

                            weaponSps.push_back(mousePos);

                            std::shared_ptr<sf::CircleShape> circle = std::make_shared<sf::CircleShape>(3);
                            circle->setFillColor(sf::Color::Green);
                            circle->setPosition(sf::Vector2f(sfMousePos));
                            circle->setOrigin(3, 3);

                            shapes.push_back(circle);

                            break;
                        }
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

        int step = 10;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) moveView(window, Vector(0, -step));
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) moveView(window, Vector(-step, 0));
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) moveView(window, Vector(0, step));
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) moveView(window, Vector(step, 0));
		
		window.clear();
		
		//draw
		drawAll(window, shapes);
		drawLines(window, lines);
		
		window.display();
	}
	
	//save in file
    saveLevel(levelFname, polygons, weaponSps, boarders);
	
	return 0;
}