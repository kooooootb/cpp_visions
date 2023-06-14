#include <filesystem>

#include "WindowHolder.h"
#include "DataNode.h"
#include "JSONDataNode.h"
#include "PathController.h"
#include "Vector.h"

WindowHolder::WindowHolder() {
    setWindow();
    view = window->getView();
}

void WindowHolder::setWindow() {
    std::filesystem::path configPath = PathController::getWindowSettings();
    std::unique_ptr<DataNode> windowNode = std::make_unique<MainDataNode>(configPath);

    unsigned int aalvl = windowNode->contains(AALVLKEY) ? static_cast<unsigned int>(windowNode->getNumber(AALVLKEY)) : 2;
    unsigned int fps = windowNode->contains(FPSKEY) ? static_cast<unsigned int>(windowNode->getNumber(FPSKEY)) : 60;
    unsigned int width = windowNode->contains(WINDOWWIDTHKEY) ? static_cast<unsigned int>(windowNode->getNumber(WINDOWWIDTHKEY)) : 640;
    unsigned int height = windowNode->contains(WINDOWHEIGHTKEY) ? static_cast<unsigned int>(windowNode->getNumber(WINDOWHEIGHTKEY)) : 480;

    sf::ContextSettings settings;
    settings.antialiasingLevel = aalvl;

    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), WINDOWLABEL, sf::Style::Default, settings);

    window->setFramerateLimit(fps);
    window->setView(sf::View(sf::FloatRect(0, 0, (float) width, (float) height)));
}

bool WindowHolder::isOpen() const {
    return window->isOpen();
}

void WindowHolder::clear() {
    return window->clear();
}

void WindowHolder::display() {
    return window->display();
}

void WindowHolder::close() {
    window->close();
}

sf::Vector2u WindowHolder::getSize() const {
    return window->getSize();
}

bool WindowHolder::pollEvent(sf::Event &event) {
    return window->pollEvent(event);
}

Point WindowHolder::getMousePositionOnLevel() const {
    return Point(window->mapPixelToCoords(sf::Mouse::getPosition(*window)));
}
