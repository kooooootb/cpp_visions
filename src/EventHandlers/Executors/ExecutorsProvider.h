#ifndef VISIONS_EXECUTORSPROVIDER_H
#define VISIONS_EXECUTORSPROVIDER_H

#include <vector>
#include <utility>

#include "Executor.h"
#include "CommandFactory.h"
#include "DataNode.h"
#include "sfEventSerializer.h"
#include "sfEventComparator.h"

class ExecutorsProvider {
private:
    std::map<sf::Event::EventType, std::vector<std::unique_ptr<Executor>>> m_executorsMap;
    std::unique_ptr<DataNode> m_dataNode; // in case someone wants to rewrite controls

    void addExecutor(std::unique_ptr<Executor> &&executor);

    explicit ExecutorsProvider(std::unique_ptr<DataNode> &&dataNode);
public:
    template<class CommandsTuple, class ...CommandsArgs>
    static std::unique_ptr<ExecutorsProvider> create(std::unique_ptr<DataNode> &&dataNode, CommandsArgs && ...commandsArgs);

    void executeByEvent(const sf::Event &);
//    virtual void setEvent(const sf::Event &) = 0;
};

template<class CommandsTuple, class... CommandsArgs>
std::unique_ptr<ExecutorsProvider> ExecutorsProvider::create(std::unique_ptr<DataNode> &&dataNode, CommandsArgs && ...commandsArgs) {
    auto executorsProvider = std::unique_ptr<ExecutorsProvider>(new ExecutorsProvider(std::move(dataNode)));
    const auto &bindingsNode = executorsProvider->m_dataNode;

    // get all commands for node
    std::vector<std::string> commands = bindingsNode->getKeys();

    // process each command
    for(const auto &commandQuery : commands){
        std::shared_ptr<Command> command = CommandFactory<CommandsTuple, CommandsArgs...>::getCommand(commandQuery, std::forward<CommandsArgs>(commandsArgs)...);

        auto eventStrings = bindingsNode->getStringsList(commandQuery);
        for(const auto &eventString : eventStrings){
            sf::Event event = sfEventSerializer::deserialize(eventString);

            auto executor = std::make_unique<Executor>(command, event);
            executorsProvider->addExecutor(std::move(executor));
        }
    }

    return executorsProvider;
}

#endif //VISIONS_EXECUTORSPROVIDER_H
