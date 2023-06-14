#include "Resize.h"

Resize::Resize(WindowHolder &window) : WindowCommand(window)
{}

void Resize::execute() {
    sf::Vector2u windowSize = m_window.getSize();
    sf::FloatRect visibleArea(0, 0, static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));
    m_window.setView(sf::View(visibleArea));
}
