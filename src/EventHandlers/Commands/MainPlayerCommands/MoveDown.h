#ifndef VISIONS_MOVEDOWN_H
#define VISIONS_MOVEDOWN_H

#include "MainPlayerCommand.h"

class MoveDown : public MainPlayerCommand{
private:

public:
    explicit MoveDown(MainPlayer &mainPlayer);

    ~MoveDown() override = default;

    void execute() override;
    static constexpr int getId();
};

constexpr int MoveDown::getId() {
    return 1;
}

#endif //VISIONS_MOVEDOWN_H
