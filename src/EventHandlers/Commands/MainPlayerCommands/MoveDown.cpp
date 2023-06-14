#include "MoveDown.h"
#include "Vector.h"

MoveDown::MoveDown(MainPlayer &mainPlayer) : MainPlayerCommand(mainPlayer)
{}

void MoveDown::execute() {
    Vector acceleration(0, -1);
    m_mainPlayer.accelerate(acceleration);
}
