#pragma once
#include "ICommand.h"
#include <vector>
#include <memory>

namespace Controller {
    class CommandHistory {
    private:
        std::vector<std::unique_ptr<ICommand> > history_;
        size_t currentIndex_;

        CommandHistory() : currentIndex_(0) {}
        CommandHistory(const CommandHistory&);
        CommandHistory& operator=(const CommandHistory&);

    public:
        static CommandHistory& getInstance() {
            static CommandHistory instance;
            return instance;
        }

        void executeCommand(std::unique_ptr<ICommand> cmd) {
            while (history_.size() > currentIndex_) {
                history_.pop_back();
            }

            cmd->execute();

            if (cmd->isUndoable()) {
                history_.push_back(std::move(cmd));
                currentIndex_++;
            }
        }

        // Undo last command
        bool undo() {
            if (currentIndex_ == 0) {
                return false;  // Nothing to undo
            }

            currentIndex_--;
            history_[currentIndex_]->undo();
            return true;
        }

        // Redo previously undone command
        bool redo() {
            if (currentIndex_ >= history_.size()) {
                return false;  // Nothing to redo
            }

            history_[currentIndex_]->execute();
            currentIndex_++;
            return true;
        }

        bool canUndo() const {
            return currentIndex_ > 0;
        }

        bool canRedo() const {
            return currentIndex_ < history_.size();
        }

        size_t getHistorySize() const {
            return history_.size();
        }

        void clear() {
            history_.clear();
            currentIndex_ = 0;
        }
    };

} // namespace Controller