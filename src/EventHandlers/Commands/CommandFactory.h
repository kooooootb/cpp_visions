#ifndef VISIONS_COMMANDFACTORY_H
#define VISIONS_COMMANDFACTORY_H

#include <string>
#include <memory>

#include "Command.h"

template<class CommandsTuple, class ...CommandsArgs>
class CommandFactory{
private:
    template<std::size_t index>
    static typename std::enable_if<index == std::tuple_size_v<CommandsTuple>, std::unique_ptr<Command>>::type
    construct(int id, [[maybe_unused]] CommandsArgs && ...args){
        (void) id; // unused
        return nullptr;
    }

    template<std::size_t index = 0>
    static typename std::enable_if<index < std::tuple_size_v<CommandsTuple>, std::unique_ptr<Command>>::type
    construct(int id, CommandsArgs && ...args){
        using CommandClass = std::tuple_element_t<index, CommandsTuple>;
        if(id == CommandClass::getId()){
            return std::make_unique<CommandClass>(args...);
        }

        return construct<index + 1>(id, args...);
    }

public:
    static std::unique_ptr<Command> getCommand(const std::string &commandQuery, CommandsArgs && ...args);
};

template<class CommandsTuple, class... CommandsArgs>
std::unique_ptr<Command>
CommandFactory<CommandsTuple, CommandsArgs...>::getCommand(const std::string &commandQuery, CommandsArgs && ...args) {
    return construct(std::stoi(commandQuery), std::forward<CommandsArgs>(args)...);
}

#endif //VISIONS_COMMANDFACTORY_H
