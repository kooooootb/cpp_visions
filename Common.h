#ifndef T5_COMMON_H
#define T5_COMMON_H

const int screen_width = 800;
const int screen_height = 600;
const int point_radius = 2;

void drawAll(sf::RenderWindow &window, const std::vector<std::shared_ptr<sf::Shape>> &shapes);

void createBackground(std::vector<std::shared_ptr<sf::Shape>> &shapes);

void initViewSector(std::shared_ptr<sf::ConvexShape> &viewShape, float viewDistance, float viewAngle);

constexpr float degToRad(float degrees){ return (float) (degrees * M_PI / 180); }
constexpr float radToDeg(float radians){ return (float) (radians * 180 / M_PI); }

inline float getAngleToZero(const sf::Vector2f &a){
	if (a.x > 0)
		return (float) acos((-1 * a.y) / sqrt(pow(a.x, 2) + pow(a.y, 2)));
	else
		return (float) (2 * M_PI - acos((-1 * a.y) / sqrt(pow(a.x, 2) + pow(a.y, 2))));
}

#endif //T5_COMMON_H
