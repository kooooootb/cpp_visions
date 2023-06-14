#include "GameState.h"
#include "GameEventHandler.h"
#include "GameManager.h"
#include "PathController.h"
#include "JSONDataNode.h"

GameState::GameState(const DataNode &dataNode) :
    manager(dataNode)
{}

std::unique_ptr<State> GameState::update(WindowHolder &window) {
    manager.update();
    manager.draw(window);

    return nullptr;
}

std::unique_ptr<EventHandler> GameState::getEventHandler(WindowHolder &window) {
    auto dataNode = MainDataNode(PathController::getKeyBindings());
    return std::make_unique<GameEventHandler>(dataNode, manager.getMainPlayer(), window);
}
