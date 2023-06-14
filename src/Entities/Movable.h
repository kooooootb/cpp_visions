#ifndef VISIONS_MOVABLE_H
#define VISIONS_MOVABLE_H

#include "Entity.h"
#include "Vector.h"
#include "Polygon.h"
#include "Types.h"

class Movable : virtual public Entity {
private:
    // current velocity, initially standing
    Vector velocity{0, 0};
    // linear stopping force
    posm_t friction;
    // maximum speed
    posm_t maxSpeed;
    // creature's acceleration
    posm_t acceleration;

    static constexpr char FRICTIONKEY[] = "friction";
    static constexpr char MAXSPEEDKEY[] = "max speed";
    static constexpr char ACCELERATIONKEY[] = "acceleration";

    bool adjustByCollision(const Polygon::Array &polygons, Vector &moveBy);
    void applyFriction(double duration);
public:
    Movable(const DataNode &dataNode);

    ~Movable() override = default;

    const Vector &getVelocity() const;
    posm_t getMaxSpeed() const;

    /**
     * Apply force to entity
     * @param direction Shows direction of acceleration
     */
    void accelerate(const Vector &direction);
    /**
     * Update position with current velocity
     * @return true if changed velocity by hitting the wall or other
     */
    bool move(const Polygon::Array &polygons);

    std::string serialize() const;
};


#endif //VISIONS_MOVABLE_H
