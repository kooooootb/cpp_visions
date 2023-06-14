#include <cmath>

#include "Rotatable.h"
#include "Vector.h"
#include "Constants.h"

Rotatable::Rotatable(const DataNode &dataNode) : Entity(dataNode) {}

void Rotatable::rotate(double rotateBy) {
    setAngle(getAngle() + rotateBy);
}

void Rotatable::lookAt(const Point &point) {
    Vector look(getPosition(), point);

    if(look.x == 0){
        if(look.y == 0){
            setAngle(0);
        } else{
            setAngle(look.y > 0 ? PI_VALUE / 2 : PI_VALUE * 3 / 2);
        }
    }else if(look.x > 0){
        setAngle(std::atan(look.y / look.x));
    }else{
        setAngle(std::atan(look.y / look.x) + PI_VALUE);
    }
}
