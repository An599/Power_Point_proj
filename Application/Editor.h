#pragma once
#include "IAction.h"
#include "../Model/Presentation.h"
#include <stack>
#include <memory>

namespace Application {

    class Editor {
    private:
        std::stack<std::unique_ptr<IAction>> undoStack_;
        std::stack<std::unique_ptr<IAction>> redoStack_;

    public:
        void doAction(std::unique_ptr<IAction> action, Model::Presentation* presentation) {
            if (!action || !presentation) {
                return;
            }

            // Execute action and get its converse (undo) action
            std::unique_ptr<IAction> converseAction = action->execute(presentation);

            if (converseAction) {
                // Clear redo stack when new action is executed
                while (!redoStack_.empty()) {
                    redoStack_.pop();
                }

                undoStack_.push(std::move(converseAction));
            }
        }

        bool undo(Model::Presentation* presentation) {
            if (undoStack_.empty() || !presentation) {
                return false;
            }

            std::unique_ptr<IAction> undoAction = std::move(undoStack_.top());
            undoStack_.pop();
            std::unique_ptr<IAction> redoAction = undoAction->execute(presentation);

            if (redoAction) {
                redoStack_.push(std::move(redoAction));
                return true;
            }

            return false;
        }
        bool redo(Model::Presentation* presentation) {
            if (redoStack_.empty() || !presentation) {
                return false;
            }
            std::unique_ptr<IAction> redoAction = std::move(redoStack_.top());
            redoStack_.pop();
            std::unique_ptr<IAction> undoAction = redoAction->execute(presentation);

            if (undoAction) {
                undoStack_.push(std::move(undoAction));
                return true;
            }

            return false;
        }
        bool canUndo() const {
            return !undoStack_.empty();
        }
        bool canRedo() const {
            return !redoStack_.empty();
        }
        void clearHistory() {
            while (!undoStack_.empty()) {
                undoStack_.pop();
            }
            while (!redoStack_.empty()) {
                redoStack_.pop();
            }
        }
    };

} 

