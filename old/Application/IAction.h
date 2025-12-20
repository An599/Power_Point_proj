#pragma once
#include "../Model/Presentation.h"
#include <memory>

namespace Application {

    // IAction - abstract interface for actions that modify the presentation
    // Each action returns its converse action for undo/redo support
    class IAction {
    public:
        virtual ~IAction() = default;

        // Execute the action on the presentation and return the converse action
        // The returned action should undo this action
        virtual std::unique_ptr<IAction> execute(Model::Presentation* presentation) = 0;
    };

} // namespace Application

