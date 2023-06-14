#ifndef VISIONS_MOVEUP_H
#define VISIONS_MOVEUP_H

#include "MainPlayerCommand.h"

class MoveUp : public MainPlayerCommand{
private:

public:
    explicit MoveUp(MainPlayer &mainPlayer);

    ~MoveUp() override = default;

    void execute() override;
    static constexpr int getId();
};

constexpr int MoveUp::getId() {
    return 0;
}

#endif //VISIONS_MOVEUP_H
