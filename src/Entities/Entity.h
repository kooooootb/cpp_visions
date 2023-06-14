#ifndef T5_ENTITY_H
#define T5_ENTITY_H

#include <SFML/Graphics.hpp>

#include <memory>
#include <filesystem>

#include "Point.h"
#include "SpriteHolder.h"
#include "Updatable.h"

/**
 * Описывает сущность, имеющую позицию на карте и физический размер (радиус круга)
 */
class Entity : public SpriteHolder , public Updatable {
private:
    static constexpr char XKEY[] = "x";
    static constexpr char YKEY[] = "y";
    static constexpr char ANGLEKEY[] = "angle";
    static constexpr char BODYRADIUSKEY[] = "body radius";

    Point position; // should be explicitly initiallized
    double angle{0}; // 0 is ok, angle is in radians
    double bodyRadius;

    bool visible = true;
protected:
    [[nodiscard]] double getAngle() const;

    virtual void setAngle(double angle_);

public:
    explicit Entity(const DataNode &dataNode);

    Entity(const Entity &entity) = default;
    Entity(Entity &&entity) = default;

    ~Entity() override = default;

    using Array = std::shared_ptr<std::vector<std::shared_ptr<Entity>>>;

    [[nodiscard]] const Point &getPosition() const;
    [[nodiscard]] double getBodyRadius() const;

    void setPosition(const Point &position_);

    std::string serialize() const;
};


#endif //T5_ENTITY_H
