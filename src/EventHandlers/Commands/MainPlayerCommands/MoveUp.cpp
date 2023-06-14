#include "Vector.h"
#include "MoveUp.h"

MoveUp::MoveUp(MainPlayer &mainPlayer) : MainPlayerCommand(mainPlayer)
{}

void MoveUp::execute() {
    Vector acceleration(0, 1);
    m_mainPlayer.accelerate(acceleration);
}
