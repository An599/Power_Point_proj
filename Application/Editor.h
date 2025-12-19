#pragma once
#include "IAction.h"
#include "../Model/Presentation.h"
#include <stack>
#include <memory>

namespace Application {

    // Editor - manages action execution and undo/redo stacks
    // Part of Application layer, not Model/Controller/View
    class Editor {
    private:
        std::stack<std::unique_ptr<IAction>> undoStack_;
        std::stack<std::unique_ptr<IAction>> redoStack_;

    public:
        // Execute an action on the presentation
        // Returns the converse action which is pushed to undo stack
        // Clears redo stack when a new action is executed
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

                // Push converse action to undo stack
                undoStack_.push(std::move(converseAction));
            }
        }

        // Undo the last action
        // Returns true if undo was successful
        bool undo(Model::Presentation* presentation) {
            if (undoStack_.empty() || !presentation) {
                return false;
            }

            // Get the last action from undo stack
            std::unique_ptr<IAction> undoAction = std::move(undoStack_.top());
            undoStack_.pop();

            // Execute undo action (which is the converse of the original action)
            std::unique_ptr<IAction> redoAction = undoAction->execute(presentation);

            if (redoAction) {
                // Push the redo action (converse of undo) to redo stack
                redoStack_.push(std::move(redoAction));
                return true;
            }

            return false;
        }

        // Redo the last undone action
        // Returns true if redo was successful
        bool redo(Model::Presentation* presentation) {
            if (redoStack_.empty() || !presentation) {
                return false;
            }

            // Get the last action from redo stack
            std::unique_ptr<IAction> redoAction = std::move(redoStack_.top());
            redoStack_.pop();

            // Execute redo action
            std::unique_ptr<IAction> undoAction = redoAction->execute(presentation);

            if (undoAction) {
                // Push the undo action (converse of redo) back to undo stack
                undoStack_.push(std::move(undoAction));
                return true;
            }

            return false;
        }

        // Check if undo is available
        bool canUndo() const {
            return !undoStack_.empty();
        }

        // Check if redo is available
        bool canRedo() const {
            return !redoStack_.empty();
        }

        // Clear all undo/redo history
        void clearHistory() {
            while (!undoStack_.empty()) {
                undoStack_.pop();
            }
            while (!redoStack_.empty()) {
                redoStack_.pop();
            }
        }
    };

} // namespace Application

