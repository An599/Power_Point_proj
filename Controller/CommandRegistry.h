#pragma once
#include "ICommandFactory.h"
#include <unordered_map>
#include <memory>
#include <string>

namespace Controller {
class CommandRegistry {
private:
    std::unordered_map<std::string, std::unique_ptr<ICommandFactory>> factories_;

    CommandRegistry() = default;
    CommandRegistry(const CommandRegistry&) = delete;
    CommandRegistry& operator=(const CommandRegistry&) = delete;

public:
    static CommandRegistry& getInstance() {
        static CommandRegistry instance;
        return instance;
    }

    void registerCommand(std::unique_ptr<ICommandFactory> factory) {
        std::string cmdName = factory->getCommandName();
        factories_[cmdName] = std::move(factory);
    }

    ICommandFactory* getFactory(const std::string& commandName) {
        auto it = factories_.find(commandName);
        if (it == factories_.end()) {
            return nullptr;
        }
        return it->second.get();
    }

    bool hasCommand(const std::string& commandName) const {
        return factories_.find(commandName) != factories_.end();
    }
};

}
