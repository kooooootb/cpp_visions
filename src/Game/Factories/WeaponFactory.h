#ifndef VISIONS_WEAPONFACTORY_H
#define VISIONS_WEAPONFACTORY_H

#include "DataNode.h"
#include "Alive.h"
#include "Weapon.h"
#include "Polygon.h"

class WeaponFactory {
private:
    static constexpr char POSITIONSKEY[] = "positions";
    static constexpr char FOLDERKEY[] = "folder";

public:
    explicit WeaponFactory() = delete;

    static std::vector<std::unique_ptr<Weapon>> construct(const DataNode &dataNode, Polygon::Array &polygons, EntityStorage &entityStorage);
};


#endif //VISIONS_WEAPONFACTORY_H
