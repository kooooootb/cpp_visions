#ifndef VISIONS_ARMED_H
#define VISIONS_ARMED_H

#include "DataNode.h"
#include "Types.h"

#include "Entity.h"
#include "Pickable.h"
#include "EntityStorage.h"

// armed with pickable entity
class Armed : virtual public Entity {
private:
    std::unique_ptr<Pickable> m_pickable = nullptr; // entity gets ownership of pointer

    std::map<int, std::pair<txtm_t, txtm_t>> m_textGunPositions;

    static constexpr char TEXTGUNPOSKEY[] = "gun positions";

    std::unique_ptr<Pickable> getNearest(EntityStorage &entityStorage) const;
public:
    explicit Armed(const DataNode &dataNode);

    Armed(const Armed &armed);
    Armed(Armed &&armed) noexcept = default;

    ~Armed() override = default;

    // checking
    [[nodiscard]] bool isArmed() const;

    // managing hands
    void take(std::unique_ptr<Pickable> &&taking); // taking chosen pickable
    std::unique_ptr<Pickable> drop();
    void change(EntityStorage &entityStorage); // swap current pickable with something nearby

    // using it
    void shoot();

    std::string serialize() const;
};


#endif //VISIONS_ARMED_H
