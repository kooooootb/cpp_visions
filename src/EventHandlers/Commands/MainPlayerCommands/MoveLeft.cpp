#include "Vector.h"
#include "MoveLeft.h"

MoveLeft::MoveLeft(MainPlayer &mainPlayer) : MainPlayerCommand(mainPlayer)
{}

void MoveLeft::execute() {
    Vector acceleration(-1, 0);
    m_mainPlayer.accelerate(acceleration);
}
