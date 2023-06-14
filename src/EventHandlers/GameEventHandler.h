#ifndef VISIONS_GAMEEVENTHANDLER_H
#define VISIONS_GAMEEVENTHANDLER_H

#include <filesystem>

#include "EventHandler.h"
#include "WindowHolder.h"
#include "ExecutorsProvider.h"
#include "CheckersProvider.h"


class GameEventHandler : public EventHandler{
private:
    std::vector<std::unique_ptr<ExecutorsProvider>> m_executorsProviders;
    std::vector<std::unique_ptr<CheckersProvider>> m_checkersProviders;

public:
    explicit GameEventHandler(DataNode &dataNode, MainPlayer &mainPlayer, WindowHolder &windowHolder);

    ~GameEventHandler() override = default;

    void pollEvents(WindowHolder &windowHolder) override;
};


#endif //VISIONS_GAMEEVENTHANDLER_H
