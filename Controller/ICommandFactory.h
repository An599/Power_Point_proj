#pragma once
#include "ICommand.h"
#include <memory>
#include <vector>
#include <string>

namespace Controller {
    class ICommandFactory {
    public:
        virtual ~ICommandFactory() = default;
        virtual std::unique_ptr<ICommand> createCommand(const std::vector<std::string>& args) = 0;
        virtual std::string getCommandName() const = 0;
    };
}
