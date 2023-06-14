#include "Vector.h"
#include "MoveRight.h"

MoveRight::MoveRight(MainPlayer &mainPlayer) : MainPlayerCommand(mainPlayer)
{}

void MoveRight::execute() {
    Vector acceleration(1, 0);
    m_mainPlayer.accelerate(acceleration);
}
