#include <memory>
#include <list>

#include "Entity.h"
#include "Common.h"

//type for kd tree
using my_kd_tree_t = nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Simple_Adaptor<float, Polygons>,Polygons, 2>;

class Player : public Entity{
private:
    float dx = 0, dy = 0;

    float health = 1;
    float damage = 100;//dealing damage

    const float friction;
    const float step;
    const float viewDistance;
    const float shapeRadius;
    const float viewAngle;

    std::vector<Edge> blockingEdges;

    void updateSpeed();
public:
    Player();
    Player(std::vector<float> args);

    ~Player() = default;

    static std::vector<float> loadCharacter(const std::string &fname);

    constexpr float getViewDistance()const { return viewDistance; }
    float getAngle()const { return angle; }
    constexpr float getViewAngle()const { return viewAngle; }
    std::vector<Edge> &getBlockingEdges() { return blockingEdges; }
    const std::vector<Edge> &getBlockingEdges() const{ return blockingEdges; }
    float getRadius() const{ return shapeRadius; }
    float getDamage() const{ return damage; }

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

    void update(Polygons &polygons, my_kd_tree_t &tree, const sf::Vector2i &mousePos, const std::vector<std::shared_ptr<Player>> &enemies,
                std::list<std::shared_ptr<sf::Shape>> &viewShape
#ifdef T5_DEBUG
                , sf::RenderWindow &window
#endif
                );

    bool takeDamage(float dealtDamage){
        if(dealtDamage >= health){
            return true;
        }else{
            health -= dealtDamage;
            return false;
        }
    }

    bool shoot(const std::vector<std::shared_ptr<Player>> &enemies, Point &where, std::shared_ptr<Player> &target);

    void setVisibility(bool visible){
        if(visible){
            shape->setFillColor(shapeColor);
        }else{
            shape->setFillColor(sf::Color::Transparent);
        }
    }
};