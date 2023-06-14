#include <cmath>

#include "Constants.h"
#include "Pickable.h"

Pickable::Pickable(const DataNode &dataNode) :
    Rotatable(dataNode) , Entity(dataNode) ,
    m_id((int) dataNode.getNumber(IDKEY))
{}

int Pickable::getId() const {
    return m_id;
}

void Pickable::rotateDropped() {
    m_currentRotation += ROTATION_STEP;
    if(m_currentRotation > PI_VALUE * 2) m_currentRotation -= PI_VALUE * 2;

    setAngle(std::sin(m_currentRotation) * MAX_ROTATION);
}

void Pickable::take() {
    m_pickedUp = true;
}

void Pickable::release() {
    m_pickedUp = false;
}

bool Pickable::mainUpdate() {
    if(m_pickedUp){
        // nothing
    } else{
        rotateDropped();
    }

    return false;
}
