#pragma once
#include "Controller/ICommandFactory.h"
#include "Controller/Commands.h"
#include <stdexcept>
#include <cctype>

namespace Controller {
    class CreatePresentationFactory : public ICommandFactory {
    public:
        std::unique_ptr<ICommand> createCommand(const std::vector<std::string>& args) override {
            if (args.size() < 2) {
                throw std::runtime_error("create_presentation requires title argument");
            }
            return std::make_unique<CreatePresentationCommand>(args[1]);
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

            return std::make_unique<LoadPresentationCommand>(filepath);
        }

        std::string getCommandName() const override {
            return "load_presentation";
        }
    };

    class SavePresentationFactory : public ICommandFactory {
    public:
        std::unique_ptr<ICommand> createCommand(const std::vector<std::string>& args) override {
            if (args.size() < 2) {
                throw std::runtime_error("save_presentation requires filename argument");
            }
            return std::make_unique<SavePresentationCommand>(args[1]);
        }

        std::string getCommandName() const override {
            return "save_presentation";
        }
    };

    class AddSlideFactory : public ICommandFactory {
    public:
        std::unique_ptr<ICommand> createCommand(const std::vector<std::string>& args) override {
            std::string title = "New Slide";
            size_t position = static_cast<size_t>(-1);
            bool hasPosition = false;

            for (size_t i = 1; i < args.size(); ++i) {
                if (!args[i].empty() && std::isdigit(args[i][0])) {
                    position = std::stoul(args[i]);
                    hasPosition = true;
                }
                else if (args[i][0] != '-') {
                    title = args[i];
                }
            }

            return std::make_unique<AddSlideCommand>(title, position, hasPosition);
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
            return std::make_unique<RemoveSlideCommand>(position);
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
            std::string text = "";
            size_t slideIndex = 0;

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
                else if (args[i] == "-text" && i + 1 < args.size()) {
                    text = args[i + 1];
                    i++;
                }
                else if (args[i] == "-slide" && i + 1 < args.size()) {
                    slideIndex = std::stoul(args[i + 1]);
                    i++;
                }
            }

            return std::make_unique<AddShapeCommand>(shapeType, x, y, width, height, color, text, slideIndex);
        }

        std::string getCommandName() const override {
            return "add_shape";
        }
    };

    class ShowFactory : public ICommandFactory {
    public:
        std::unique_ptr<ICommand> createCommand(const std::vector<std::string>&) override {
            return std::make_unique<ShowCommand>();
        }

        std::string getCommandName() const override {
            return "show";
        }
    };

    class HelpFactory : public ICommandFactory {
    public:
        std::unique_ptr<ICommand> createCommand(const std::vector<std::string>&) override {
            return std::make_unique<HelpCommand>();
        }

        std::string getCommandName() const override {
            return "help";
        }
    };

    class ExitFactory : public ICommandFactory {
    public:
        std::unique_ptr<ICommand> createCommand(const std::vector<std::string>&) override {
            return std::make_unique<ExitCommand>();
        }

        std::string getCommandName() const override {
            return "exit";
        }
    };

} 
