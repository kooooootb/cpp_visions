#ifndef VISIONS_STATE_H
#define VISIONS_STATE_H

#include <memory>

#include "WindowHolder.h"
#include "EventHandler.h"
#include "DataNode.h"

class State{
public:
    virtual ~State() = default;

    virtual std::unique_ptr<State> update(WindowHolder &windowHolder) = 0;
    virtual std::unique_ptr<EventHandler> getEventHandler(WindowHolder &window) = 0; // state can change window, e.g. close it
};

#endif //VISIONS_STATE_H
