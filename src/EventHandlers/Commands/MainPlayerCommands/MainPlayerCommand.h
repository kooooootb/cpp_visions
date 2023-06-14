#ifndef VISIONS_MAINPLAYERCOMMAND_H
#define VISIONS_MAINPLAYERCOMMAND_H

#include "Command.h"
#include "MainPlayer.h"

class MainPlayerCommand : public Command{
protected:
    explicit MainPlayerCommand(MainPlayer &mainPlayer);

    MainPlayer &m_mainPlayer;

public:
    ~MainPlayerCommand() override = default;
};


#endif //VISIONS_MAINPLAYERCOMMAND_H
