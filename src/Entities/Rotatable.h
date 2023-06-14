#ifndef VISIONS_ROTATABLE_H
#define VISIONS_ROTATABLE_H

#include "MainDataNode.h"
#include "Entity.h"

class Rotatable : virtual public Entity {
public:
    explicit Rotatable(const DataNode &dataNode);

    ~Rotatable() override = default;

    void rotate(double rotateBy);
    void lookAt(const Point &point);
};


#endif //VISIONS_ROTATABLE_H
