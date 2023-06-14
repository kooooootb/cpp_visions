#ifndef VISIONS_DRAWABLE_H
#define VISIONS_DRAWABLE_H

#include <SFML/Graphics.hpp>

#include "WindowHolder.h"

class Drawable{
public:
    virtual ~Drawable() = default;

    virtual void draw(WindowHolder &window) const = 0;
};

#endif //VISIONS_DRAWABLE_H
