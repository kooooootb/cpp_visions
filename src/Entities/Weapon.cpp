#include <cmath>

#include "Weapon.h"
#include "Polygon.h"

Weapon::Weapon(const DataNode &dataNode, Polygon::Array &polygons_, EntityStorage &entityStorage) :
    Entity(dataNode) , Pickable(dataNode) ,
    m_ammo((int) dataNode.getNumber(AMMOKEY))
{
    // set referenced projectile
    m_refProjectile = std::make_shared<Projectile>(*dataNode.getDerived(PROJECTILEKEY), polygons_, entityStorage);
}

Weapon::Weapon(const Weapon &weapon) :
    Entity(weapon) , Pickable(weapon) ,
    m_ammo(weapon.m_ammo) , m_refProjectile(weapon.m_refProjectile)
{}

bool Weapon::spendBullet() {
    if(m_ammo < 1){
        return false;
    } else{
        m_ammo -= 1;
        return true;
    }
}

bool Weapon::use() {
    if(!spendBullet()){
        return false;
    }

    // duplicate projectile
    std::unique_ptr<Projectile> projectile = createProjectile();

    // register projectile
    m_projectiles.push_back(std::move(projectile));

    return true;
}

std::unique_ptr<Projectile> Weapon::createProjectile() {
    return std::make_unique<Projectile>(*m_refProjectile);
}

bool Weapon::mainUpdate() {
    Pickable::mainUpdate();

    for(auto &projectile : m_projectiles){
        projectile->update();
    }

    return false;
}

void Weapon::draw(WindowHolder &windowHolder) const {
    SpriteHolder::draw(windowHolder);

    for(const auto &projectile : m_projectiles){
        projectile->draw(windowHolder);
    }
}
