#include "GameEventHandler.h"
#include "MainDataNode.h"

GameEventHandler::GameEventHandler(DataNode &dataNode, MainPlayer &mainPlayer, WindowHolder &windowHolder) {
    std::unique_ptr<DataNode> mainPlayerNode = dataNode.getDerived(MAINPLAYERPROVIDERSKEY);
    std::unique_ptr<DataNode> windowNode = dataNode.getDerived(WINDOWPROVIDERSKEY);

    m_executorsProviders.push_back(ExecutorsProvider::create<MainPlayerExecutorsCommands>(mainPlayerNode->getDerived(EXECUTORSPROVIDERKEY), mainPlayer, windowHolder));
    m_executorsProviders.push_back(ExecutorsProvider::create<WindowExecutorsCommands>(windowNode->getDerived(EXECUTORSPROVIDERKEY), windowHolder));

    m_checkersProviders.push_back(CheckersProvider::create<MainPlayerCheckersCommands>(mainPlayerNode->getDerived(CHECKERSPROVIDERKEY), mainPlayer));
    m_checkersProviders.push_back(CheckersProvider::create<WindowExecutorsCommands>(windowNode->getDerived(CHECKERSPROVIDERKEY), windowHolder));
}

void GameEventHandler::pollEvents(WindowHolder &windowHolder) {
    while(windowHolder.pollEvent(m_currentEvent)){
        for(const auto &executorsProvider : m_executorsProviders){
            executorsProvider->executeByEvent(m_currentEvent);
        }
    }

    for(const auto &checkersProvider : m_checkersProviders){
        checkersProvider->executeCheckers();
    }
}
