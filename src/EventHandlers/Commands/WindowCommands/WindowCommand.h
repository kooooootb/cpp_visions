#ifndef VISIONS_WINDOWCOMMAND_H
#define VISIONS_WINDOWCOMMAND_H

#include "Command.h"
#include "WindowHolder.h"

class WindowCommand : public Command {
protected:
    WindowCommand(WindowHolder &window);

    WindowHolder &m_window;

public:
    ~WindowCommand() override = default;
};


#endif //VISIONS_WINDOWCOMMAND_H
