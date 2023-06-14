#include <cmath>
#include <utility>

#include "MainDataNode.h"
#include "Armed.h"
#include "Vector.h"

Armed::Armed(const DataNode &dataNode) :
    Entity(dataNode) , m_textGunPositions(dataNode.getPairsMap(TEXTGUNPOSKEY))
{}

Armed::Armed(const Armed &armed) : Entity(armed) , m_textGunPositions(armed.m_textGunPositions) {
    m_pickable = nullptr;
}

std::unique_ptr<Pickable> Armed::getNearest(EntityStorage &entityStorage) const {
    std::unique_ptr<Pickable> result = nullptr;

    auto pickables = entityStorage.iteratePickables();
    for(const auto &pickable : pickables){
        if(Vector(getPosition(), pickable.second.getPosition()).sqr() < std::pow(getBodyRadius() + pickable.second.getBodyRadius(), 2)){
            result = entityStorage.popPickable(pickable.first);

            break;
        }
    }

    return result;
}

bool Armed::isArmed() const {
    return m_pickable != nullptr;
}

std::unique_ptr<Pickable> Armed::drop() {
    if(m_pickable != nullptr){
        m_pickable->setPosition(getPosition());
    }

    std::unique_ptr<Pickable> dropped = std::move(m_pickable);
    m_pickable = nullptr;

    if(dropped != nullptr){
        dropped->release(); // say weapon that we dropped it
    }

    resetSprite(); // reset to normal sprite

    return dropped;
}

void Armed::take(std::unique_ptr<Pickable> &&taking){
    m_pickable = std::move(taking);

    m_pickable->take(); // say weapon that we took it

    repositionTexture(m_textGunPositions[m_pickable->getId()]); // set sprite
}

void Armed::change(EntityStorage &entityStorage) {
    auto dropping = drop();
    if(dropping != nullptr){
        entityStorage.queueAddPickable(std::move(dropping));
    }

    auto nearest = getNearest(entityStorage);
    if(nearest != nullptr){
        take(std::move(nearest));
    }
}

void Armed::shoot() {
    if(m_pickable != nullptr){
        m_pickable->use();
    }
}

std::string Armed::serialize() const {
    using Formatter = JSONDataNode;
    return Formatter::formatPairsMap(TEXTGUNPOSKEY, m_textGunPositions);
}
