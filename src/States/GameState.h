#ifndef VISIONS_GAMESTATE_H
#define VISIONS_GAMESTATE_H

#include "DataNode.h"
#include "WindowHolder.h"
#include "State.h"
#include "EventHandler.h"
#include "GameManager.h"

class GameState : public State {
private:
    GameManager manager;

public:
    explicit GameState(const DataNode &dataNode);

    ~GameState() override = default;

    std::unique_ptr<State> update(WindowHolder &window) override;
    std::unique_ptr<EventHandler> getEventHandler(WindowHolder &window) override;
};


#endif //VISIONS_GAMESTATE_H
