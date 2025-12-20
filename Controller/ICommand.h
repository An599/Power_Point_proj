#pragma once

namespace Controller {

    class ICommand {
    public:
        virtual ~ICommand() {}
        virtual void execute() = 0;
        virtual void undo() = 0;
        virtual bool isUndoable() const = 0;
    };

}
