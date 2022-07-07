#include <SFML/Graphics.hpp>

#include "nanoflann.hpp"

#include <vector>
#include <memory>
#include <iostream>

#include "file.h"
#include "Common.h"
#include "Player.h"

//type for kd tree
using my_kd_tree_t = nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Simple_Adaptor<float, Polygons>,Polygons, 2>;

int main(){
	//level save file
	const static std::string fname = "level.bin";
	const static unsigned int fps = 60;
	
	//init window
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), std::to_string(fps), sf::Style::Default, settings);
	window.setFramerateLimit(fps);
	
	//init arrays
	std::vector<std::shared_ptr<sf::Shape>> defShapes;//default shapes (player and background)
	
	//set background
	createBackground(defShapes);
	
	//load polygons from file
	Polygons polygons = std::move(loadLevelForTree(fname));
	std::vector<std::shared_ptr<sf::Shape>> convexShapes = polygons.collectShapes();
	
	//set tree
	my_kd_tree_t tree(2, polygons, {10 /* max leaf */});
	
	//set player
	Player player;
	defShapes.push_back(player.getPlayerShape());
//	defShapes.push_back(player.getViewShape());
	
	//variables:
	bool moveByMouse = false;
	sf::Vector2i mousePos;
	sf::Clock clock;
	sf::Time tick = sf::seconds(1.0 / fps);
	
	sf::Event event{};
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			switch (event.type){
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::MouseButtonPressed:
					mousePos = sf::Mouse::getPosition(window);
					moveByMouse = true;
					break;
				case sf::Event::MouseButtonReleased:
					moveByMouse = false;
					break;
				case sf::Event::MouseMoved:
					if(moveByMouse){
						sf::Vector2i newMousePos = sf::Mouse::getPosition(window);
						player.move(newMousePos - mousePos);
						mousePos = newMousePos;
					}
					break;
				case sf::Event::KeyPressed:
					switch(event.key.code){
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
		
		while(clock.getElapsedTime() < tick);
		clock.restart();
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) player.accelerate(0, -1);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) player.accelerate(-1, 0);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) player.accelerate(0, 1);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) player.accelerate(1, 0);
		player.update(polygons, tree, sf::Mouse::getPosition(window));
		
		window.clear();
		
		//draw section
		drawAll(window, defShapes);//draw default shapes
		drawAll(window, convexShapes);//draw forms' shapes
		
		window.display();
	}
	
	return 0;
}