#include "MainPlayer.h"

#include <utility>

MainPlayer::MainPlayer(const DataNode &dataNode, Polygon::Array &polygons, EntityStorage &entityStorage, std::shared_ptr<sf::Texture> texture) :
    Alive(dataNode) , Movable(dataNode) , Armed(dataNode) , Visioning(dataNode) , Entity(dataNode) , Rotatable(dataNode) ,
    m_polygons(polygons) , m_entityStorage(entityStorage) , m_viewingTexture(std::move(texture))
{}

bool MainPlayer::mainUpdate() {
    if(!isAlive()) {
        return true;
    }

    move(m_polygons);
    regenerate();
    updateViewShape(m_polygons);
    updateSeeingEntities(m_entityStorage);

    return false;
}

void MainPlayer::draw(WindowHolder &window) const {
    drawViewShape(window, *m_viewingTexture);
    SpriteHolder::draw(window);
}

void MainPlayer::pickup() {
    change(m_entityStorage);
}


