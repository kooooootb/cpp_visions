#include <cassert>

#include "PathController.h"

std::unique_ptr<PathController> PathController::m_object = nullptr;

PathController::PathController(std::filesystem::path rootFolder) : m_rootFolder(std::move(rootFolder)) {}

void PathController::initialize(std::filesystem::path rootFolder) {
    m_object = std::make_unique<PathController>(std::move(rootFolder));
}

bool PathController::isInitialized() {
    return m_object != nullptr;
}

std::filesystem::path PathController::getKeyBindings() {
    assert(isInitialized());
    return m_object->m_rootFolder / SETTINGSFOLDER / KEYBINDINGSFILE;
}

std::filesystem::path PathController::getWindowSettings() {
    assert(isInitialized());
    return m_object->m_rootFolder / SETTINGSFOLDER / WINDOWCONFIGFILE;
}

std::filesystem::path PathController::getEntityConfig(const std::string &entityName) {
    assert(isInitialized());
    return m_object->m_rootFolder / ENTITIESFOLDER / entityName / ENTITIESCONFIG;
}

std::filesystem::path PathController::getEntityFolder(const std::string &entityName) {
    assert(isInitialized());
    return m_object->m_rootFolder / ENTITIESFOLDER / entityName;
}

std::filesystem::path PathController::getLevelConfig(const std::string &levelName) {
    assert(isInitialized());
    return m_object->m_rootFolder / LEVELFOLDER / levelName / LEVELCONFIG;
}

std::filesystem::path PathController::getLevelFolder(const std::string &levelName) {
    assert(isInitialized());
    return m_object->m_rootFolder / LEVELFOLDER / levelName;
}


