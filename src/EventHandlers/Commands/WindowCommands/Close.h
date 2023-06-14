#ifndef VISIONS_CLOSE_H
#define VISIONS_CLOSE_H

#include "WindowCommand.h"

class Close : public WindowCommand {
private:

public:
    explicit Close(WindowHolder &window);

    ~Close() override = default;

    void execute() override;
    static constexpr int getId();
};

constexpr int Close::getId() {
    return 5;
}

#endif //VISIONS_CLOSE_H
