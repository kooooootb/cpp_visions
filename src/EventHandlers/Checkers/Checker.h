#ifndef VISIONS_CHECKER_H
#define VISIONS_CHECKER_H

#include <SFML/Graphics.hpp>

#include <memory>

#include "Command.h"

class Checker {
private:
    sf::Keyboard::Key m_referencedKey;
    std::shared_ptr<Command> m_command;

public:
    Checker(std::shared_ptr<Command> command, sf::Keyboard::Key key);

    [[nodiscard]] const sf::Keyboard::Key &getKey() const;
    void checkAndExecute();

};

#endif //VISIONS_CHECKER_H
