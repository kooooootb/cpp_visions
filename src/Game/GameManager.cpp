#include "GameManager.h"
#include "Watcher.h"
#include "JSONDataNode.h"
#include "Weapon.h"
#include "EnemyFactory.h"
#include "WeaponFactory.h"
#include "PathController.h"

GameManager::GameManager(const DataNode &dataNode) :
    m_polygons(std::make_shared<std::vector<std::unique_ptr<Polygon>>>()) ,
    m_levelWidth(static_cast<decltype(m_levelWidth)>(dataNode.getNumber(LEVELWIDTHKEY))) ,
    m_levelHeight(static_cast<decltype(m_levelHeight)>(dataNode.getNumber(LEVELHEIGHTKEY)))
{
    initializeWeapons(dataNode);
    initializeEnemies(dataNode);
    initializePolygons(dataNode);
    initializeUpdaters(dataNode);

    m_entityStorage.cleanup();

    m_mainPlayer = std::make_unique<MainPlayer>(MainDataNode(PathController::getEntityConfig(dataNode.getString(PLAYERSCFGKEY))), m_polygons, m_entityStorage, m_polygons->front()->getTexture());
}

void GameManager::initializePolygons(const DataNode &dataNode) {
    // initializing polygons
    auto polygonsDataNodes = dataNode.getNodesList(POLYGONSKEY);

    for(const auto &polygonDataNode : polygonsDataNodes){
        auto polygon = std::make_unique<Polygon>(*polygonDataNode);
        m_polygons->push_back(std::move(polygon));
    }

    // shade first polygon as it is background
    m_polygons->front()->setColor(sf::Color(200, 200, 200));
}

void GameManager::initializeWeapons(const DataNode &dataNode) {
    // initializing weapons
    auto weaponsDataNodes = dataNode.getNodesList(WEAPONSKEY);
    for(const auto &weaponDataNode : weaponsDataNodes){
        auto weapons = WeaponFactory::construct(*weaponDataNode, m_polygons, m_entityStorage);
        for(auto &weapon : weapons){
            addPickable(std::move(weapon));
        }
    }
}

void GameManager::initializeEnemies(const DataNode &dataNode) {
    // initializing enemies
    auto enemiesDataNodes = dataNode.getNodesList(ENEMIESKEY);
    for(const auto &enemyDataNode : enemiesDataNodes){
        auto enemies = EnemyFactory::construct(*enemyDataNode, m_polygons, m_entityStorage);
        for(auto &enemy : enemies){
            addAlive(std::move(enemy));
        }
    }
}

void GameManager::initializeUpdaters(const DataNode &dataNode){
    auto updatersCount = static_cast<size_t>(dataNode.getNumber(UPDATERSCOUNTKEY));

    m_updaters.reserve(updatersCount);
    for(size_t i = 0;i < updatersCount;++i){
        m_updaters.push_back(std::make_unique<Updater>());
    }

    refillUpdaters();
}

void GameManager::addPickable(std::unique_ptr<Pickable> &&pickable){
    m_entityStorage.queueAddPickable(std::move(pickable));
}

void GameManager::addAlive(std::unique_ptr<Alive> &&alive){
    m_entityStorage.queueAddAlive(std::move(alive));
}

void GameManager::runUpdaters() {
    if(m_entityStorage.hasChanged()){
        refillUpdaters();
        m_entityStorage.resetHasChanged();
    }

    for(auto &updater : m_updaters){
        updater->runUpdate();
    }

    for(auto &updater : m_updaters){
        updater->waitUntilDone();
    }

    m_entityStorage.cleanup();
}

void GameManager::refillUpdaters() {
    for(auto &updater : m_updaters){
        updater->resetUpdatables();
    }

    size_t index = 0;
    auto entities = m_entityStorage.iterateEntities();
    for(const auto &entityPair : entities){
        m_updaters[index % m_updaters.size()]->addUpdatable(&entityPair.second);
    }
}

void GameManager::update() {
    runUpdaters();

    m_mainPlayer->update();
}

void GameManager::draw(WindowHolder &window) {
    for(const auto &polygon : *m_polygons){
        polygon->draw(window);
    }

    m_mainPlayer->draw(window);

    auto entitiesIterator = m_entityStorage.iterateEntities();
    for(const auto &entity : entitiesIterator){
        entity.second.draw(window);
    }

    window.updateViewWithMouse(m_mainPlayer->getPosition(), m_levelWidth, m_levelHeight);
}

const MainPlayer &GameManager::getMainPlayer() const {
    return *m_mainPlayer;
}

MainPlayer &GameManager::getMainPlayer() {
    return *m_mainPlayer;
}
