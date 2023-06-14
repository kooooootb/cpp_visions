#include <utility>

#include "Executor.h"
#include "sfEventComparator.h"

Executor::Executor(std::shared_ptr<Command> command, sf::Event event) :
    m_command(std::move(command)) , m_referencedEvent(event)
{}

const sf::Event &Executor::getEvent() const {
    return m_referencedEvent;
}

void Executor::checkAndExecute(const sf::Event &event) {
    if(sfEventComparator::equal(m_referencedEvent, event)){
        m_command->execute();
    }
}
