#ifndef VISIONS_GAMEMANAGER_H
#define VISIONS_GAMEMANAGER_H

#include <memory>

#include "Polygon.h"
#include "Entity.h"
#include "Updater.h"
#include "EntityStorage.h"
#include "MainPlayer.h"
#include "VertexHolder.h"
#include "WindowHolder.h"

class GameManager {
private:
    // main storages
    Polygon::Array m_polygons; // will be drawn, used by every entity
    EntityStorage m_entityStorage;

    // updaters
    std::vector<std::unique_ptr<Updater>> m_updaters;

    // main player
    std::unique_ptr<MainPlayer> m_mainPlayer;

    // level parameters
    unsigned int m_levelWidth, m_levelHeight;

    static constexpr char WEAPONSKEY[] = "weapons";
    static constexpr char ENEMIESKEY[] = "enemies";
    static constexpr char POLYGONSKEY[] = "polygons";
    static constexpr char PLAYERSCFGKEY[] = "config";
    static constexpr char LEVELWIDTHKEY[] = "level width";
    static constexpr char LEVELHEIGHTKEY[] = "level height";
    static constexpr char UPDATERSCOUNTKEY[] = "updaters";

    void initializePolygons(const DataNode &dataNode);
    void initializeWeapons(const DataNode &dataNode);
    void initializeEnemies(const DataNode &dataNode);
    void initializeUpdaters(const DataNode &dataNode);

    void addPickable(std::unique_ptr<Pickable> &&weapon);
    void addAlive(std::unique_ptr<Alive> &&enemy);

    void runUpdaters();
    void refillUpdaters();
public:
    explicit GameManager(const DataNode &dataNode);

    [[nodiscard]] const MainPlayer &getMainPlayer() const;
    MainPlayer &getMainPlayer();

    void update();
    void draw(WindowHolder &window);
};


#endif //VISIONS_GAMEMANAGER_H
