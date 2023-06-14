#include "Pickup.h"

Pickup::Pickup(MainPlayer &mainPlayer, const WindowHolder &window) : MainPlayerCommand(mainPlayer) , m_window(window)
{}

void Pickup::execute() {
    m_mainPlayer.pickup();
}
