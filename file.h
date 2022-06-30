#ifndef T5_FILE_H
#define T5_FILE_H

#include "Polygon.h"

void saveLevel(const std::string &fname, const std::vector<std::shared_ptr<std::list<sf::Vector2f>>> &polygons);

void loadLevel(std::vector<std::shared_ptr<sf::Shape>> &shapes, const std::string &fname, std::vector<std::shared_ptr<std::list<sf::Vector2f>>> &polygons);

Polygons loadLevelForTree(const std::string &fname);

#endif //T5_FILE_H
