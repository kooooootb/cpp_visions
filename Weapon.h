#ifndef T5_WEAPON_H
#define T5_WEAPON_H

#include <vector>

#include "Entity.h"

class Weapon : public Entity{
private:
    unsigned int ammo;
    const float damage, speed;

public:
    Weapon(std::pair<std::string, std::vector<float>> args, const Point &point);

    float getDamage() const{ return damage; }
    float getSpeed() const{ return speed; }
    unsigned int getAmmo() const{ return ammo; }
    const std::string &getName() const{ return name; }

    std::string name;

    static std::pair<std::string, std::vector<float>> loadEntity(const std::string &fname);

    bool shoot(std::shared_ptr<Player> &target, Point &where, const std::vector<Edge> &blockingEdges);//true if killed, not empty
    void shoot();
    bool spendBullet();
    bool empty() const{ return ammo < 1; }
};



class Weapons{//class for tree functions
private:
    const std::vector<std::shared_ptr<Weapon>> &weapons;
public:
    Weapons(const std::vector<std::shared_ptr<Weapon>> &w) : weapons(w) {}

    //------------------------------------------------------------------
    //Functions for nanoflann's kdtree:
    inline size_t kdtree_get_point_count() const { return weapons.size(); }

    inline float kdtree_get_pt(const size_t idx, const size_t dim) const
    {
        if (dim == 0)
            return weapons[idx]->getPosition().x;
        else
            return weapons[idx]->getPosition().y;
    }

    template <class BBOX>
    bool kdtree_get_bbox(BBOX& /* bb */) const
    {
        return false;
    }
    //------------------------------------------------------------------
};

#endif //T5_WEAPON_H
