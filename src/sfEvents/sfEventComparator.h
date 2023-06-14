#ifndef VISIONS_SFEVENTCOMPARATOR_H
#define VISIONS_SFEVENTCOMPARATOR_H

#include "SFML/Graphics.hpp"

class sfEventComparator {
public:
    sfEventComparator() = delete;

    static bool equal(const sf::Event &event1, const sf::Event &event2);
};


#endif //VISIONS_SFEVENTCOMPARATOR_H
