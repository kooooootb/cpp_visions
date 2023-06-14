#include <utility>

#include "WindowHandler.h"
#include "GameState.h"
#include "JSONDataNode.h"
#include "PathController.h"

WindowHandler::WindowHandler(std::filesystem::path rootPath)
{
    PathController::initialize(std::move(rootPath));

    m_windowHolder = std::make_unique<WindowHolder>();
    setInitialState();
}

void WindowHandler::setInitialState() {
    std::unique_ptr<DataNode> levelNode = std::make_unique<MainDataNode>(PathController::getLevelConfig("level1"));
    auto initialState = std::make_unique<GameState>(*levelNode);
    setState(std::move(initialState));
}

void WindowHandler::setState(std::unique_ptr<State> &&state) {
    m_state = std::move(state);
    m_eventHandler = std::move(m_state->getEventHandler(*m_windowHolder));
}

void WindowHandler::run() {
    while(m_windowHolder->isOpen()){
        m_eventHandler->pollEvents(*m_windowHolder);

        m_windowHolder->clear();

        std::unique_ptr<State> newState = m_state->update(*m_windowHolder);
        if(newState != nullptr){
            setState(std::move(newState));
        }

        m_windowHolder->display();
    }
}
