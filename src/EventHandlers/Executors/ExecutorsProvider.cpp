#include <utility>

#include "ExecutorsProvider.h"

ExecutorsProvider::ExecutorsProvider(std::unique_ptr<DataNode> &&dataNode) : m_dataNode(std::move(dataNode))
{}

void ExecutorsProvider::addExecutor(std::unique_ptr<Executor> &&executor) {
    sf::Event::EventType eventType = executor->getEvent().type;
    auto it = m_executorsMap.find(eventType);
    if(it == m_executorsMap.end()){
        // add as new
        auto &newVector = m_executorsMap.insert({eventType, std::vector<std::unique_ptr<Executor>>()}).first->second;
        newVector.push_back(std::move(executor));
    } else{
        it->second.push_back(std::move(executor));
    }
}

void ExecutorsProvider::executeByEvent(const sf::Event &event) {
    auto it = m_executorsMap.find(event.type);
    if(it != m_executorsMap.end()){
        auto &vector = it->second;
        for(const auto &execPtr : vector){
            execPtr->checkAndExecute(event);
        }
    }
}
