#ifndef VISIONS_MOVERIGHT_H
#define VISIONS_MOVERIGHT_H

#include "MainPlayerCommand.h"

class MoveRight : public MainPlayerCommand{
private:

public:
    explicit MoveRight(MainPlayer &mainPlayer);

    ~MoveRight() override = default;

    void execute() override;
    static constexpr int getId();
};

constexpr int MoveRight::getId() {
    return 3;
}

#endif //VISIONS_MOVERIGHT_H
