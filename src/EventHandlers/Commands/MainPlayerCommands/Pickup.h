#ifndef VISIONS_PICKUP_H
#define VISIONS_PICKUP_H

#include "MainPlayerCommand.h"

class Pickup : public MainPlayerCommand{
private:
    const WindowHolder &m_window;

public:
    explicit Pickup(MainPlayer &mainPlayer, const WindowHolder &window);

    ~Pickup() override = default;

    void execute() override;
    static constexpr int getId();
};

constexpr int Pickup::getId() {
    return 7;
}

#endif //VISIONS_PICKUP_H
