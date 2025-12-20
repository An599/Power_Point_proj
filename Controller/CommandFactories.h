#pragma once
#include "ICommandFactory.h"
#include "Commands.h"
#include "CommandUnRe.h"
#include <stdexcept>
#include <cctype>

namespace Controller {

    class CreatePresentationFactory : public ICommandFactory {
    public:
        std::unique_ptr<ICommand> createCommand(const std::vector<std::string>& args) override {
            if (args.size() < 2) {
                throw std::runtime_error("create_presentation requires title argument");
            }

            std::string title = args[1];
            for (size_t i = 2; i < args.size(); ++i) {
                title += " " + args[i];
            }

            return std::unique_ptr<ICommand>(new CreatePresentationCommand(title));
        }

        std::string getCommandName() const override {
            return "create_presentation";
        }
    };

    class LoadPresentationFactory : public ICommandFactory {
    public:
        std::unique_ptr<ICommand> createCommand(const std::vector<std::string>& args) override {
            std::string filepath;

            for (size_t i = 1; i < args.size(); ++i) {
                if (args[i] == "-path" && i + 1 < args.size()) {
                    filepath = args[i + 1];
                    break;
                }
            }

            if (filepath.empty()) {
                throw std::runtime_error("load_presentation requires -path argument");
            }

            return std::unique_ptr<ICommand>(new LoadPresentationCommand(filepath));
        }

        std::string getCommandName() const override {
            return "load_presentation";
        }
    };

    class SavePresentationFactory : public ICommandFactory {
    public:
        std::unique_ptr<ICommand> createCommand(const std::vector<std::string>& args) override {
            // Default filename if not provided
            std::string filename = "presentation.json";
            if (args.size() >= 2 && !args[1].empty()) {
                filename = args[1];
            }
            return std::unique_ptr<ICommand>(new SavePresentationCommand(filename));
        }

        std::string getCommandName() const override {
            return "save_presentation";
        }
    };

    class AddSlideFactory : public ICommandFactory {
    public:
        std::unique_ptr<ICommand> createCommand(const std::vector<std::string>& args) override {
            size_t position = static_cast<size_t>(-1);
            bool hasPosition = false;

            for (size_t i = 1; i < args.size(); ++i) {
                const std::string& arg = args[i];

                if (!arg.empty() && std::isdigit(static_cast<unsigned char>(arg[0]))) {
                    try {
                        position = std::stoul(arg);
                        hasPosition = true;
                    }
                    catch (...) {
                    }
                }
            }

            return std::unique_ptr<ICommand>(new AddSlideCommand(position, hasPosition));
        }

        std::string getCommandName() const override {
            return "add_slide";
        }
    };

    class RemoveSlideFactory : public ICommandFactory {
    public:
        std::unique_ptr<ICommand> createCommand(const std::vector<std::string>& args) override {
            if (args.size() < 2) {
                throw std::runtime_error("remove_slide requires position argument");
            }
            size_t position = std::stoul(args[1]);
            return std::unique_ptr<ICommand>(new RemoveSlideCommand(position));
        }

        std::string getCommandName() const override {
            return "remove_slide";
        }
    };

    class AddShapeFactory : public ICommandFactory {
    public:
        std::unique_ptr<ICommand> createCommand(const std::vector<std::string>& args) override {
            if (args.size() < 2) {
                throw std::runtime_error("add_shape requires shape_type argument");
            }

            std::string shapeType = args[1];
            int x = 0, y = 0, width = 100, height = 100;
            std::string color = "black";
            std::string fillColor = "none";
            std::string text = "";
            size_t slideIndex = 0;
            bool toFront = false;

            for (size_t i = 2; i < args.size(); ++i) {
                if (args[i] == "-coord" && i + 2 < args.size()) {
                    x = std::stoi(args[i + 1]);
                    y = std::stoi(args[i + 2]);
                    i += 2;
                }
                else if (args[i] == "-size" && i + 2 < args.size()) {
                    width = std::stoi(args[i + 1]);
                    height = std::stoi(args[i + 2]);
                    i += 2;
                }
                else if (args[i] == "-color" && i + 1 < args.size()) {
                    color = args[i + 1];
                    i++;
                }
                else if (args[i] == "-fill" && i + 1 < args.size()) {
                    fillColor = args[i + 1];
                    i++;
                }
                else if (args[i] == "-text" && i + 1 < args.size()) {
                    text = args[i + 1];
                    i++;
                }
                else if (args[i] == "-slide" && i + 1 < args.size()) {
                    slideIndex = std::stoul(args[i + 1]);
                    i++;
                }
                else if (args[i] == "-front") {
                    toFront = true;
                }
            }

            return std::unique_ptr<ICommand>(new AddShapeCommand(
                shapeType, x, y, width, height, color, fillColor, text, slideIndex, toFront));
        }

        std::string getCommandName() const override {
            return "add_shape";
        }
    };

    class AddTextFactory : public ICommandFactory {
    public:
        std::unique_ptr<ICommand> createCommand(const std::vector<std::string>& args) override {
            int x = 0, y = 0, width = 100, height = 50;
            std::string text = "";
            std::string color = "black";
            size_t slideIndex = 0;
            bool toFront = false;

            for (size_t i = 1; i < args.size(); ++i) {
                if (args[i] == "-coord" && i + 2 < args.size()) {
                    x = std::stoi(args[i + 1]);
                    y = std::stoi(args[i + 2]);
                    i += 2;
                }
                else if (args[i] == "-size" && i + 2 < args.size()) {
                    width = std::stoi(args[i + 1]);
                    height = std::stoi(args[i + 2]);
                    i += 2;
                }
                else if (args[i] == "-text" && i + 1 < args.size()) {
                    text = args[i + 1];
                    i++;
                }
                else if (args[i] == "-color" && i + 1 < args.size()) {
                    color = args[i + 1];
                    i++;
                }
                else if (args[i] == "-slide" && i + 1 < args.size()) {
                    slideIndex = std::stoul(args[i + 1]);
                    i++;
                }
                else if (args[i] == "-front") {
                    toFront = true;
                }
            }

            if (text.empty()) {
                throw std::runtime_error("add_text requires -text argument");
            }

            return std::unique_ptr<ICommand>(new AddTextCommand(
                x, y, width, height, text, color, slideIndex, toFront));
        }

        std::string getCommandName() const override {
            return "add_text";
        }
    };

    class ShowFactory : public ICommandFactory {
    public:
        std::unique_ptr<ICommand> createCommand(const std::vector<std::string>&) override {
            return std::unique_ptr<ICommand>(new ShowCommand());
        }

        std::string getCommandName() const override {
            return "show";
        }
    };

    class UndoFactory : public ICommandFactory {
    public:
        std::unique_ptr<ICommand> createCommand(const std::vector<std::string>&) override {
            return std::unique_ptr<ICommand>(new UndoCommand());
        }

        std::string getCommandName() const override {
            return "undo";
        }
    };

    class RedoFactory : public ICommandFactory {
    public:
        std::unique_ptr<ICommand> createCommand(const std::vector<std::string>&) override {
            return std::unique_ptr<ICommand>(new RedoCommand());
        }

        std::string getCommandName() const override {
            return "redo";
        }
    };

    class DuplicateSlideFactory : public ICommandFactory {
    public:
        std::unique_ptr<ICommand> createCommand(const std::vector<std::string>& args) override {
            if (args.size() < 2) {
                throw std::runtime_error("duplicate_slide requires position argument");
            }
            size_t position = std::stoul(args[1]);
            return std::unique_ptr<ICommand>(new DuplicateSlideCommand(position));
        }

        std::string getCommandName() const override {
            return "duplicate_slide";
        }
    };

    class MoveSlideFactory : public ICommandFactory {
    public:
        std::unique_ptr<ICommand> createCommand(const std::vector<std::string>& args) override {
            if (args.size() < 3) {
                throw std::runtime_error("move_slide requires from_position and to_position arguments");
            }
            size_t fromPosition = std::stoul(args[1]);
            size_t toPosition = std::stoul(args[2]);
            return std::unique_ptr<ICommand>(new MoveSlideCommand(fromPosition, toPosition));
        }

        std::string getCommandName() const override {
            return "move_slide";
        }
    };

    class HelpFactory : public ICommandFactory {
    public:
        std::unique_ptr<ICommand> createCommand(const std::vector<std::string>&) override {
            return std::unique_ptr<ICommand>(new HelpCommand());
        }

        std::string getCommandName() const override {
            return "help";
        }
    };

    class ExitFactory : public ICommandFactory {
    public:
        std::unique_ptr<ICommand> createCommand(const std::vector<std::string>&) override {
            return std::unique_ptr<ICommand>(new ExitCommand());
        }

        std::string getCommandName() const override {
            return "exit";
        }
    };

} 
