#ifndef VISIONS_ENEMYFACTORY_H
#define VISIONS_ENEMYFACTORY_H

#include "DataNode.h"
#include "Entity.h"
#include "Enemy.h"
#include "Polygon.h"

class EnemyFactory {
private:
    static constexpr char POSITIONSKEY[] = "positions";
    static constexpr char FOLDERKEY[] = "folder";

public:
    explicit EnemyFactory() = delete;

    static std::vector<std::unique_ptr<Enemy>> construct(const DataNode &dataNode, Polygon::Array &polygons, EntityStorage &entityStorage);
};

#endif //VISIONS_ENEMYFACTORY_H
