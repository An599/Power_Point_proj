#pragma once

namespace Controller {
    class ICommand {
    public:
        virtual ~ICommand() = default;
        virtual void execute() = 0;
    };
}
