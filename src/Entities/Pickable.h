#ifndef VISIONS_PICKABLE_H
#define VISIONS_PICKABLE_H

#include "MainDataNode.h"
#include "Rotatable.h"

class Pickable : public Rotatable {
private:
    bool m_pickedUp = false;
    int m_id;
    double m_currentRotation = 0;

    static constexpr char IDKEY[] = "id";

    // for rotating while dropped
    static constexpr double MAX_ROTATION = 30;
    static constexpr double ROTATION_STEP = 0.01;
    void rotateDropped();

public:
    explicit Pickable(const DataNode &dataNode);

    Pickable(const Pickable &) = default;
    Pickable(Pickable &&) = default;

    ~Pickable() override = default;

    [[nodiscard]] int getId() const;

    void take(); // what happens when player takes it
    void release(); // what happens when player releases it

    virtual bool use() = 0;

    bool mainUpdate() override;
};


#endif //VISIONS_PICKABLE_H
