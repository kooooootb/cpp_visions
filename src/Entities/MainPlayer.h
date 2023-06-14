#ifndef VISIONS_MAINPLAYER_H
#define VISIONS_MAINPLAYER_H

#include "Alive.h"
#include "Movable.h"
#include "Rotatable.h"
#include "Armed.h"
#include "Visioning.h"

#include "Polygon.h"
#include "EntityStorage.h"

class MainPlayer : virtual public Alive, virtual public Movable, virtual public Rotatable,
        virtual public Armed, virtual public Visioning {
private:
    Polygon::Array &m_polygons;
    EntityStorage &m_entityStorage;

    std::shared_ptr<sf::Texture> m_viewingTexture;

public:
    MainPlayer(const DataNode &dataNode, Polygon::Array &polygons, EntityStorage &entityStorage, std::shared_ptr<sf::Texture> texture);

    MainPlayer(const MainPlayer &) = default;
    MainPlayer(MainPlayer &&) = default;

    ~MainPlayer() override = default;

    void pickup();

    bool mainUpdate() override;
    void draw(WindowHolder &window) const override;
};


#endif //VISIONS_MAINPLAYER_H
