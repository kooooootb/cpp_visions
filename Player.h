#include "SFML/Graphics.hpp"
#include "nanoflann.hpp"

#include <memory>

#include "headers.h"
#include "Common.h"

//type for kd tree
using my_kd_tree_t = nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Simple_Adaptor<float, Polygons>,Polygons, 2>;

class Player{
private:
    Point position;
    float dx = 0, dy = 0;
    float angle = 0;

    constexpr static float friction = 0.5;
    constexpr static float step = 1;
    constexpr static float viewDistance = 300;
    constexpr static float shapeRadius = 6;
    constexpr static float viewAngle = degToRad(20);

    std::shared_ptr<sf::CircleShape> playerShape;
    std::shared_ptr<sf::ConvexShape> viewShape;
public:
    Player();

    ~Player() = default;

    float getX()const { return position.x; }
    float getY()const { return position.y; }
    const Point &getPosition()const { return position; }
    float getViewDistance()const { return viewDistance; }
    float getAngle()const { return angle; }
    float getViewAngle()const { return viewAngle; }
    const std::shared_ptr<sf::CircleShape> &getPlayerShape() const { return playerShape; }
    const std::shared_ptr<sf::ConvexShape> &getViewShape() const { return viewShape; }
    std::shared_ptr<sf::ConvexShape> &getViewShape() { return viewShape; }

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

    void update(Polygons &polygons, my_kd_tree_t &tree, const sf::Vector2i &mousePos
#ifdef T5_DEBUG
                , sf::RenderWindow &window
#endif
                );
};