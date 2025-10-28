#pragma once

namespace Controller {

    // Command interface - part of Controller
    class ICommand {
    public:
        virtual ~ICommand() = default;
        virtual void execute() = 0;
    };
}