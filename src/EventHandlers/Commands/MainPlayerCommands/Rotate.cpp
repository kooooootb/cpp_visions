#include "Rotate.h"

Rotate::Rotate(MainPlayer &mainPlayer, const WindowHolder &window) : MainPlayerCommand(mainPlayer) , m_window(window)
{}

void Rotate::execute() {
    m_mainPlayer.lookAt(m_window.getMousePositionOnLevel());
}
