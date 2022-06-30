#ifndef T5_COMMON_H
#define T5_COMMON_H

const int screen_width = 800;
const int screen_height = 600;
const int point_radius = 2;

void drawAll(sf::RenderWindow &window, const std::vector<std::shared_ptr<sf::Shape>> &shapes);

void createBackground(std::vector<std::shared_ptr<sf::Shape>> &shapes);

#endif //T5_COMMON_H
