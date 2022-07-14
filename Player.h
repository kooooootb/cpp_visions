#include <memory>
#include <list>

#include "Entity.h"
#include "Common.h"

class Player : public Entity{
private:
    float dx = 0, dy = 0;

    float health;

    std::shared_ptr<Weapon> activeWeapon;

    const float friction;
    const float step;
    const float viewDistance;
    const float shapeRadius;
    const float viewAngle;
    const float maxHealth;
    const float maxSpeed;

    std::vector<Edge> blockingEdges;

    void updateSpeed();
    void collisionCheck(Polygons &polygons, KDPolygonsTree &tree);//check if current dx dy push player into wall
    void updatePosition();
    void updateView(Polygons &polygons, const KDPolygonsTree &tree, const std::vector<std::shared_ptr<Entity>> &entities, std::list<std::shared_ptr<sf::Shape>> &viewShape
#ifdef T5_DEBUG
            , sf::RenderWindow &window
#endif
    );

    void pickWeapon(KDWeaponsTree &tree, std::vector<std::shared_ptr<Weapon>> &weapons);
public:
    Player();
    Player(std::vector<float> args);

    ~Player() = default;

    static std::vector<float> loadEntity(const std::string &fname);

    constexpr float getViewDistance()const { return viewDistance; }
    constexpr float getViewAngle()const { return viewAngle; }
    std::vector<Edge> &getBlockingEdges() { return blockingEdges; }
    const std::vector<Edge> &getBlockingEdges() const{ return blockingEdges; }
    float getRadius() const{ return shapeRadius; }
    unsigned int getAmmo() const;

    bool isArmed() const{ return activeWeapon != nullptr; }

    void accelerate(float dX, float dY);
    void move(sf::Vector2i vector);

    inline void updateCoord(float &coord, float &d, float limit);

    void update(Polygons &polygons, KDPolygonsTree &tree, const sf::Vector2i &mousePos, const std::vector<std::shared_ptr<Entity>> &entities,
                std::list<std::shared_ptr<sf::Shape>> &viewShape
#ifdef T5_DEBUG
                , sf::RenderWindow &window
#endif
                );

    bool takeDamage(float dealtDamage);

    bool shoot(std::vector<std::shared_ptr<Player>> &players, std::shared_ptr<Projectile> &projectile, const KDPolygonsTree &tree, Polygons &polygons);//true if able to shoot
    void changeWeapon(KDWeaponsTree &tree, std::vector<std::shared_ptr<Weapon>> &weapons);
};