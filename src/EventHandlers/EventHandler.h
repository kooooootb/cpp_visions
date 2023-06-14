#ifndef VISIONS_EVENTHANDLER_H
#define VISIONS_EVENTHANDLER_H

#include "WindowHolder.h"

// commands ahead
// for main player
#include "MoveUp.h"
#include "MoveDown.h"
#include "MoveLeft.h"
#include "MoveRight.h"
#include "Rotate.h"
#include "Pickup.h"
// for window
#include "Close.h"
#include "Resize.h"

class EventHandler {
protected:
    sf::Event m_currentEvent{};

    // known commands for executors
    using MainPlayerExecutorsCommands = std::tuple<
            Rotate, Pickup
    >;
    using WindowExecutorsCommands = std::tuple<
            Close, Resize
    >;

    // known commands for checkers
    using MainPlayerCheckersCommands = std::tuple<
            MoveLeft, MoveUp, MoveRight, MoveDown
    >;

    // basic separatings
    static constexpr char EXECUTORSPROVIDERKEY[] = "executors";
    static constexpr char CHECKERSPROVIDERKEY[] = "checkers";

    // different classes of handlers
    static constexpr char MAINPLAYERPROVIDERSKEY[] = "main player";
    static constexpr char WINDOWPROVIDERSKEY[] = "window";

public:
    virtual ~EventHandler() = default;

    virtual void pollEvents(WindowHolder &windowHolder) = 0;
};


#endif //VISIONS_EVENTHANDLER_H
