#include <SFML/Graphics.hpp>

#include "nanoflann.hpp"

#include <vector>
#include <memory>
#include <iostream>

#include "file.h"
#include "Common.h"

//type for kd tree
using my_kd_tree_t = nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Simple_Adaptor<float, Polygons>,Polygons, 2>;

class Player{
private:
	float x = (float) screen_width / 2;
	float y = (float) screen_height / 2;
	float dx = 0, dy = 0;
	constexpr static float friction = 0.5;
	constexpr static float step = 1;
	constexpr static float viewDistance = 50;
	constexpr static float shapeRadius = 6;
	constexpr static float viewAngle = 6;
	
	std::shared_ptr<sf::CircleShape> playerShape;
	std::shared_ptr<sf::CircleShape> viewShape;
public:
	Player() {
		playerShape = std::make_shared<sf::CircleShape>(shapeRadius);
		playerShape->setFillColor(sf::Color(226, 143, 40));
		playerShape->setPosition(x, y);
		playerShape->setOrigin(shapeRadius, shapeRadius);
		
		viewShape = std::make_shared<sf::CircleShape>(viewDistance);
		viewShape->setFillColor(sf::Color::Transparent);
		viewShape->setOutlineColor(sf::Color(255, 51, 33));
		viewShape->setOutlineThickness(1);
		viewShape->setPosition(x, y);
		viewShape->setOrigin(viewDistance, viewDistance);
	}
	
	~Player() = default;
	
	float getX()const { return x; }
	float getY()const { return y; }
	const std::shared_ptr<sf::CircleShape> &getPlayerShape() const { return playerShape; }
	const std::shared_ptr<sf::CircleShape> &getViewShape() const { return viewShape; }
	
	void accelerate(float dX, float dY){
		this->dx += dX * step;
		this->dy += dY * step;
	}
	
	void move(sf::Vector2i vector){
		this->x += (float) vector.x;
		this->y += (float) vector.y;
	}
	
	static void updateCoord(float &coord, float &d, float limit){
		coord += d;
		
		if(coord >= limit){
			coord = limit;
			d = 0;
		}
		else if(coord <= 0){
			coord = 0;
			d = 0;
		}
	}
	
	void update(Polygons &polygons, my_kd_tree_t &tree){
		//set player coords:
		updateCoord(x, dx, screen_width);
		updateCoord(y, dy, screen_height);
		
		dx = std::abs(dx) > friction ? (dx > 0 ? dx - friction : dx + friction) : 0;
		dy = std::abs(dy) > friction ? (dy > 0 ? dy - friction : dy + friction) : 0;
		
		playerShape->setPosition(x, y);
		viewShape->setPosition(x, y);
		
		//set forms' shapes:
		std::vector<std::pair<uint32_t, float>> ret_matches;
		
		const float query[2] = {x, y};
		nanoflann::SearchParams params;
		
		const size_t nMatches = tree.radiusSearch(query, viewDistance * viewDistance, ret_matches, params);
		
		polygons.hideAll();
		for(auto &pair : ret_matches){
			polygons[pair.first].polygon.makeVisible();
		}
		
		polygons.updateVisibility();
		
//		std::cout << nMatches << std::endl;
	}
};

int main(){
	//level save file
	const static std::string fname = "level.bin";
	
	//init window
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "15", sf::Style::Default, settings);
	window.setFramerateLimit(60);
	
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
	defShapes.push_back(player.getViewShape());
	
	//variables:
	bool moveByMouse = false;
	sf::Vector2i mousePos;
	
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
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) player.accelerate(0, -1);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) player.accelerate(-1, 0);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) player.accelerate(0, 1);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) player.accelerate(1, 0);
		player.update(polygons, tree);
		
		window.clear();
		
		//draw section
		drawAll(window, defShapes);//draw default shapes
		drawAll(window, convexShapes);//draw forms' shapes
		
		window.display();
	}
	
	return 0;
}