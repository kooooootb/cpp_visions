#ifndef VISIONS_CHECKERSPROVIDER_H
#define VISIONS_CHECKERSPROVIDER_H

#include <vector>
#include <utility>

#include "Checker.h"
#include "MainDataNode.h"
#include "CommandFactory.h"
#include "sfEventSerializer.h"

class CheckersProvider{
private:
    std::vector<std::unique_ptr<Checker>> m_checkers;
    std::unique_ptr<DataNode> m_dataNode; // in case someone wants to rewrite controls

    void addChecker(std::unique_ptr<Checker> &&checker);

    explicit CheckersProvider(std::unique_ptr<DataNode> &&dataNode);
public:
    template<class CommandsTuple, class ...CommandsArgs>
    static std::unique_ptr<CheckersProvider> create(std::unique_ptr<DataNode> &&dataNode, CommandsArgs &&... commandsArgs);

    void executeCheckers();
//    virtual void setEvent(const sf::Event &) = 0;
};

template<class CommandsTuple, class... CommandsArgs>
std::unique_ptr<CheckersProvider> CheckersProvider::create(std::unique_ptr<DataNode> &&dataNode, CommandsArgs &&... commandsArgs) {
    auto checkersProvider = std::unique_ptr<CheckersProvider>(new CheckersProvider(std::move(dataNode)));
    const auto &bindingsNode = checkersProvider->m_dataNode;

    // get all commands for node
    std::vector<std::string> commands = bindingsNode->getKeys();

    // process each command
    for(const auto &commandQuery : commands){
        std::shared_ptr<Command> command = CommandFactory<CommandsTuple, CommandsArgs...>::getCommand(commandQuery, commandsArgs...);

        auto keyStrings = bindingsNode->getStringsList(commandQuery);
        for(const auto &keyString : keyStrings){
            sf::Keyboard::Key key = sfEventSerializer::stringToKeyboardKey(keyString);

            auto checker = std::make_unique<Checker>(command, key);
            checkersProvider->addChecker(std::move(checker));
        }
    }

    return checkersProvider;
}

#endif //VISIONS_CHECKERSPROVIDER_H
