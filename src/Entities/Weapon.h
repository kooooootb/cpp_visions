#ifndef T5_WEAPON_H
#define T5_WEAPON_H

#include <vector>

#include "EntityStorage.h"
#include "Pickable.h"
#include "DataNode.h"
#include "Projectile.h"
#include "WindowHolder.h"

class Weapon : public Pickable {
private:
    // weapon properties
    int m_ammo;

    // projectiles
    std::vector<std::unique_ptr<Projectile>> m_projectiles;
    std::shared_ptr<Projectile> m_refProjectile; // чтобы не загружать текстуры постоянно, reference projectile

    static constexpr char AMMOKEY[] = "ammo";
    static constexpr char PROJECTILEKEY[] = "projectile";

    bool spendBullet();
    std::unique_ptr<Projectile> createProjectile();
public:
    Weapon(const DataNode &dataNode, Polygon::Array &polygons_, EntityStorage &entityStorage);
    Weapon(const Weapon &weapon);

    ~Weapon() override = default;

    bool use() override; // true if shot, false if cannot

    bool mainUpdate() override;
    void draw(WindowHolder &windowHolder) const override;
};

#endif //T5_WEAPON_H
