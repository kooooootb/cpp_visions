#include <utility>

#include "Checker.h"

Checker::Checker(std::shared_ptr<Command> command, sf::Keyboard::Key key) :
    m_command(std::move(command)) , m_referencedKey(key)
{}

const sf::Keyboard::Key &Checker::getKey() const {
    return m_referencedKey;
}

void Checker::checkAndExecute() {
    if(sf::Keyboard::isKeyPressed(m_referencedKey)){
        m_command->execute();
    }
}
