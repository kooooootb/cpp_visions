#ifndef VISIONS_COMMAND_H
#define VISIONS_COMMAND_H

class Command {
public:
    virtual ~Command() = default;

    virtual void execute() = 0;
};

#endif //VISIONS_COMMAND_H
