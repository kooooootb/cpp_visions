#include "nanoflann.hpp"

#include "Player.h"
#include "Polygon.h"

Player::Player() : position((float) screen_width / 2, (float) screen_height / 2) {
    playerShape = std::make_shared<sf::CircleShape>(shapeRadius);
    playerShape->setFillColor(sf::Color(226, 143, 40));
    playerShape->setPosition(position.x, position.y);
    playerShape->setOrigin(shapeRadius, shapeRadius);

		viewShape = std::make_shared<sf::ConvexShape>(viewDistance);
		viewShape->setFillColor(sf::Color(228, 228, 228));
		viewShape->setOutlineColor(sf::Color(200, 200, 200));
		viewShape->setOutlineThickness(1);
//		viewShape->setPosition(x, y);

//		initViewSector(viewShape, viewDistance, viewAngle);
}

void
Player::update(Polygons &polygons, my_kd_tree_t &tree, const sf::Vector2i &mousePos
#ifdef T5_DEBUG
               , sf::RenderWindow &window
#endif
               ) {
    //rotate view sector:
    angle = getAngleToZero(sf::Vector2f((float) mousePos.x - position.x, (float) mousePos.y - position.y));

    //set player coords:
    updateCoord(position.x, dx, screen_width);
    updateCoord(position.y, dy, screen_height);

    dx = std::abs(dx) > friction ? (dx > 0 ? dx - friction : dx + friction) : 0;
    dy = std::abs(dy) > friction ? (dy > 0 ? dy - friction : dy + friction) : 0;

    playerShape->setPosition(position.x, position.y);
//		viewShape->setPosition(x, y);

    //set forms' shapes:
    std::vector<std::pair<uint32_t, float>> ret_matches;

    const float query[2] = {position.x, position.y};
    nanoflann::SearchParams params;

    const size_t nMatches = tree.radiusSearch(query, viewDistance * viewDistance, ret_matches, params);

    polygons.hideAll();
    for(auto &pair : ret_matches){
        polygons[pair.first].polygon.makeVisible();
    }

    polygons.updateVisibility(*this
#ifdef T5_DEBUG
                              , window
#endif
                              );

//		std::cout << nMatches << std::endl;
}
