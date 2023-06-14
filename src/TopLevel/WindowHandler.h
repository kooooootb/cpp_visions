#ifndef VISIONS_WINDOWHANDLER_H
#define VISIONS_WINDOWHANDLER_H

#include <memory>
#include <filesystem>

#include "WindowHolder.h"
#include "EventHandler.h"
#include "State.h"

class WindowHandler {
private:
    std::unique_ptr<WindowHolder> m_windowHolder;
    std::unique_ptr<EventHandler> m_eventHandler;
    std::unique_ptr<State> m_state;

    void setInitialState();
public:
    explicit WindowHandler(std::filesystem::path rootPath);

    void setState(std::unique_ptr<State> &&state);

    void run();
};


#endif //VISIONS_WINDOWHANDLER_H
