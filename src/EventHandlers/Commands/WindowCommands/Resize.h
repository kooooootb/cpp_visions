#ifndef VISIONS_RESIZE_H
#define VISIONS_RESIZE_H

#include "WindowCommand.h"

class Resize : public WindowCommand {
private:

public:
    explicit Resize(WindowHolder &window);

    ~Resize() override = default;

    void execute() override;
    static constexpr int getId();
};

constexpr int Resize::getId() {
    return 6;
}

#endif //VISIONS_RESIZE_H
