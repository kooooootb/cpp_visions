#include <SFML/Graphics.hpp>

#include <memory>
#include <map>
#include <vector>
#include <list>

#include "Entity.h"
#include "Common.h"

class BasePlayer : public Entity{
private:
    double dx = 0, dy = 0;

    unsigned int health;

    std::shared_ptr<Weapon> activeWeapon;

    std::map<std::string, sf::Texture> armedTextures;

protected:
    const double friction;
    const double step;
    const double viewDistance;
    const double shapeRadius;
    const double viewAngle;
    const int maxHealth;
    const double maxSpeed;

    std::vector<Edge> blockingEdges;

    void updateSpeed();
    void collisionCheck(const std::vector<std::shared_ptr<Polygon>> &polygons);//check if current dx dy push player into wall
    void updatePosition(const std::vector<std::shared_ptr<Polygon>> &polygons);

    void pickWeapon(std::vector<std::shared_ptr<Weapon>> &weapons);

public:
    BasePlayer(const std::string &name, double x_, double y_, double friction_, double step_,
               double viewDistance_, double shapeRadius_, double viewAngle_, int maxHealth_, double maxSpeed_);

    ~BasePlayer() = default;

    double getViewDistance()const { return viewDistance; }
    double getViewAngle()const { return viewAngle; }
    double getRadius() const{ return shapeRadius; }
    unsigned int getAmmo() const;

    bool isArmed() const{ return activeWeapon != nullptr; }

    void accelerate(float dX, float dY);
    void move(sf::Vector2f vector);

    inline void updateCoord(double &coord, double &d);

    bool takeDamage(unsigned int dealtDamage);
    bool shoot(std::vector<std::shared_ptr<BasePlayer>> &players, std::shared_ptr<Projectile> &projectile, const std::vector<std::shared_ptr<Polygon>> &polygons);//true if able to shoot
    void changeWeapon(std::vector<std::shared_ptr<Weapon>> &weapons);

    virtual void update(const std::vector<std::shared_ptr<Polygon>> &polygons, const sf::Vector2f &mousePos,
                const std::vector<std::shared_ptr<Entity>> &entities) = 0;
}; // class BasePlayer

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Player: public BasePlayer{
private:
    sf::VertexArray viewShape;

    void updateView(const std::vector<std::shared_ptr<Polygon>> &polygons, const std::vector<std::shared_ptr<Entity>> &entities);
    PointType getPointType(const Point &point, const std::array<Vector, 3> &views)const;

public:
    Player(const std::string &name_, double x_, double y_, double friction_, double step_, double viewDistance_,
                      double shapeRadius_, double viewAngleDeg_, int maxHealth_, double maxSpeed_);

    static Player loadPlayer(const std::string &fname);

    const sf::VertexArray &getViewShape() const{ return viewShape; }
    sf::VertexArray &getViewShape() { return viewShape; }

    void update(const std::vector<std::shared_ptr<Polygon>> &polygons, const sf::Vector2f &mousePos,
                const std::vector<std::shared_ptr<Entity>> &entities) final;
}; // class Player

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Enemy: public BasePlayer{
public:
    Enemy(const std::string &name_, double x_, double y_, double friction_, double step_, double viewDistance_,
           double shapeRadius_, double viewAngleDeg_, int maxHealth_, double maxSpeed_);

    static Enemy loadEnemy(const std::string &fname);

    void update(const std::vector<std::shared_ptr<Polygon>> &polygons, const sf::Vector2f &mousePos,
                const std::vector<std::shared_ptr<Entity>> &entities) final;


}; // class Enemy