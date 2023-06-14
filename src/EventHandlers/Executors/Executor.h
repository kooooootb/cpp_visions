#ifndef VISIONS_EXECUTOR_H
#define VISIONS_EXECUTOR_H

#include <SFML/Graphics.hpp>

#include <memory>

#include "Command.h"

class Executor {
private:
    sf::Event m_referencedEvent;
    std::shared_ptr<Command> m_command;

public:
    Executor(std::shared_ptr<Command> command, sf::Event event);

    [[nodiscard]] const sf::Event &getEvent() const;
    void checkAndExecute(const sf::Event &event);

};


#endif //VISIONS_EXECUTOR_H
