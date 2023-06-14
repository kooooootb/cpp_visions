#ifndef VISIONS_ALIVE_H
#define VISIONS_ALIVE_H

#include "Entity.h"

/**
 * Описывает живую сущность
 */
class Alive : virtual public Entity {
private:
    using health_t = double;
    // main health
    health_t health;
    // maximum health
    health_t maxHealth;
    // regenerative speed, how much at a time
    health_t regenSpeed;
    bool dead = false;

    static constexpr char HEALTHKEY[] = "health";
    static constexpr char REGENSPEEDKEY[] = "regenerative speed";

public:
    explicit Alive(const DataNode &dataNode);

    ~Alive() override = default;

    using Array = std::shared_ptr<std::vector<std::shared_ptr<Alive>>>;

    /**
     * Reduce health by damage
     * @param damage damage
     * @return true if creature dies (, false otherwise
     */
    bool takeDamage(health_t damage);
    /**
     * Regenerate health
     */
    void regenerate();
    /**
     * @return true if alive
     */
    bool isAlive() const;

    std::string serialize() const;
};

#endif //VISIONS_ALIVE_H
