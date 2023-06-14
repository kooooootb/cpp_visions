#ifndef VISIONS_MOVELEFT_H
#define VISIONS_MOVELEFT_H

#include "MainPlayerCommand.h"

class MoveLeft : public MainPlayerCommand{
private:

public:
    explicit MoveLeft(MainPlayer &mainPlayer);

    ~MoveLeft() override = default;

    void execute() override;
    static constexpr int getId();
};

constexpr int MoveLeft::getId() {
    return 2;
}

#endif //VISIONS_MOVELEFT_H
