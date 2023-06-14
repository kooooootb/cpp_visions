#include "sfEventComparator.h"

bool sfEventComparator::equal(const sf::Event &event1, const sf::Event &event2) {
    if(event1.type != event2.type){
        return false;
    }

    if(event1.type == sf::Event::KeyPressed || event1.type == sf::Event::KeyReleased){
        if(event1.key.code != event2.key.code){
            return false;
        }
    } else if(event1.type == sf::Event::MouseButtonPressed || event1.type == sf::Event::MouseButtonReleased){
        if(event1.mouseButton.button != event2.mouseButton.button){
            return false;
        }
    }

    return true;
}
