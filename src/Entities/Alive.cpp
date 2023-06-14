#include "Alive.h"
#include "JSONDataNode.h"

Alive::Alive(const DataNode &dataNode) : Entity(dataNode) {
    maxHealth = dataNode.getNumber(HEALTHKEY);
    regenSpeed = dataNode.getNumber(REGENSPEEDKEY);

    health = maxHealth;
}

void Alive::regenerate(){
    auto restoring = static_cast<unsigned int>(static_cast<double>(regenSpeed) * getDuration());

    if(maxHealth - health < restoring){
        health = maxHealth;
    } else{
        health += restoring;
    }
}

bool Alive::takeDamage(health_t damage){
    if(damage >= health){
        dead = true;
        return true;
    } else{
        health -= damage;
        return false;
    }
}

bool Alive::isAlive() const {
    return !dead;
}

std::string Alive::serialize() const {
    using Formatter = JSONDataNode;
    return  Formatter::formatNumber(HEALTHKEY, maxHealth) + "," +
            Formatter::formatNumber(REGENSPEEDKEY, regenSpeed);
}
