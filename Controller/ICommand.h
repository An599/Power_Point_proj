
#pragma once

namespace Controller {

    // ICommand - primitive interface with undo capability
    // Sufficient - provides execute and undo operations
    // Decoupled - doesn't depend on specific command types
    class ICommand {
    public:
        virtual ~ICommand() {}
        virtual void execute() = 0;
        virtual void undo() = 0;
        virtual bool isUndoable() const = 0;  // Completeness - some commands may not be undoable
    };

}