#include "SFML/Graphics.hpp"
#include "nanoflann.hpp"

#include <memory>
#include <list>

#include "Common.h"

//type for kd tree
using my_kd_tree_t = nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Simple_Adaptor<float, Polygons>,Polygons, 2>;

class Player{
private:
    Point position;
    float dx = 0, dy = 0;
    float angle = 0;

    constexpr static float friction = FRICTION;
    constexpr static float step = STEP;
    constexpr static float viewDistance = VIEWDISTANCE;
    constexpr static float shapeRadius = SHAPERADIUS;
    constexpr static float viewAngle = degToRad(VIEWANGLEDEG);

    std::shared_ptr<sf::CircleShape> playerShape;
public:
    Player();

    ~Player() = default;

    float getX()const { return position.x; }
    float getY()const { return position.y; }
    const Point &getPosition()const { return position; }
    constexpr float getViewDistance()const { return viewDistance; }
    float getAngle()const { return angle; }
    constexpr float getViewAngle()const { return viewAngle; }
    const std::shared_ptr<sf::CircleShape> &getPlayerShape() const { return playerShape; }

    void accelerate(float dX, float dY){
        this->dx += dX * step;
        this->dy += dY * step;
    }

    void move(sf::Vector2i vector){
        this->position.x += (float) vector.x;
        this->position.y += (float) vector.y;
    }

    inline static void updateCoord(float &coord, float &d, float limit){
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

    std::list<std::shared_ptr<sf::Shape>> update(Polygons &polygons, my_kd_tree_t &tree, const sf::Vector2i &mousePos
#ifdef T5_DEBUG
                , sf::RenderWindow &window
#endif
                );
};