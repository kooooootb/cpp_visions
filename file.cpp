#include <SFML/Graphics.hpp>

#include <list>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>

#include "file.h"

void saveLevel(const std::string &fname, const std::vector<std::shared_ptr<std::list<sf::Vector2f>>> &polygons){
	std::ofstream fd(fname, std::ios::out | std::ios::binary | std::ios::trunc);
	
	if(fd.is_open()){
		auto size = polygons.size();
		fd.write(reinterpret_cast<char *>(&size), sizeof(size));
		
		for(auto &it : polygons){
			std::list<sf::Vector2f> &list = *it;
			size = list.size();
			fd.write(reinterpret_cast<char *>(&size), sizeof(size));
			
			for(auto &point : list){
				fd.write(reinterpret_cast<char *>(&point.x), sizeof(point.x));
				fd.write(reinterpret_cast<char *>(&point.y), sizeof(point.y));
			}
		}
		
		fd.close();
	}
}

void loadLevel(std::vector<std::shared_ptr<sf::Shape>> &shapes, const std::string &fname, std::vector<std::shared_ptr<std::list<sf::Vector2f>>> &polygons){
	std::ifstream fd(fname, std::ios::in | std::ios::binary);
	
	if(fd.is_open()){
		unsigned long long polygonsSize;
		fd.read(reinterpret_cast<char *>(&polygonsSize), sizeof(polygonsSize));
		
		for(int i = 0;i < polygonsSize;++i){
			polygons.emplace_back(std::make_shared<std::list<sf::Vector2f>>());
			
			std::list<sf::Vector2f> &list = *polygons.back();
			
			unsigned long long listSize;
			fd.read(reinterpret_cast<char *>(&listSize), sizeof(listSize));
			
			for(int j = 0;j < listSize;++j){
				float x, y;
				fd.read(reinterpret_cast<char *>(&x), sizeof(x));
				fd.read(reinterpret_cast<char *>(&y), sizeof(y));
				list.emplace_back(x, y);
			}
		}
		
		fd.close();
	}
	
	if(!polygons.empty()){
		for(const auto &it : polygons){
			const std::list<sf::Vector2f> &list = *it;
			
			sf::ConvexShape convex;
			convex.setPointCount(list.size());
			convex.setFillColor(sf::Color::Black);
			int i = 0;
			for(const auto &point : list){
				convex.setPoint(i++, point);
			}
			shapes.push_back(std::make_shared<sf::ConvexShape>(convex));
		}
	}
}

Polygons loadLevelForTree(const std::string &fname){
	std::ifstream fd(fname, std::ios::in | std::ios::binary);
	
	Polygons pols;
	
	if(fd.is_open()){
		unsigned long long polygonsAmount;
		fd.read(reinterpret_cast<char *>(&polygonsAmount), sizeof(polygonsAmount));
		
		for(int i = 0; i < polygonsAmount; ++i){
			Polygon pol;
			
			unsigned long long bpAmount;
			fd.read(reinterpret_cast<char *>(&bpAmount), sizeof(bpAmount));
			
			for(int j = 0; j < bpAmount; ++j){
				float x, y;
				fd.read(reinterpret_cast<char *>(&x), sizeof(x));
				fd.read(reinterpret_cast<char *>(&y), sizeof(y));
				pol.addBPoint(Point(pol, x, y));
			}
			pol.endAdding();
			
			pols.add(pol);
		}
		
		fd.close();
	}
	
	if(!pols.empty()){
		for(auto &it : pols){
			it.setConvex();
		}
	}
	
	return pols;
}