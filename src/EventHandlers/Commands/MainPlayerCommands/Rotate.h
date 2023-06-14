#ifndef VISIONS_ROTATE_H
#define VISIONS_ROTATE_H

#include "MainPlayerCommand.h"

class Rotate : public MainPlayerCommand{
private:
    const WindowHolder &m_window;

public:
    explicit Rotate(MainPlayer &mainPlayer, const WindowHolder &window);

    ~Rotate() override = default;

    void execute() override;
    static constexpr int getId();
};

constexpr int Rotate::getId() {
    return 4;
}

#endif //VISIONS_ROTATE_H
