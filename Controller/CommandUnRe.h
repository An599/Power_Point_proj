#pragma once
#include "Commands.h"
#include "../Viewer/View.h"
#include "../Application/Application.h"
#include "../Model/Model.h"

namespace Controller {
    inline void UndoCommand::execute() {
        auto& app = Application::Application::getInstance();
        auto& model = Model::Model::getInstance();
        auto& view = View::ViewFacade::getInstance();

        if (!model.hasPresentation()) {
            view.showError("No presentation loaded.");
            return;
        }

        if (app.getEditor().undo(model.getPresentation())) {
            view.showSuccess("Undo successful");
        }
        else {
            view.showWarning("Nothing to undo");
        }
    }

    inline void RedoCommand::execute() {
        auto& app = Application::Application::getInstance();
        auto& model = Model::Model::getInstance();
        auto& view = View::ViewFacade::getInstance();

        if (!model.hasPresentation()) {
            view.showError("No presentation loaded.");
            return;
        }

        if (app.getEditor().redo(model.getPresentation())) {
            view.showSuccess("Redo successful");
        }
        else {
            view.showWarning("Nothing to redo");
        }
    }

} 
