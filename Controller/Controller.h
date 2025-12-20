#pragma once
#include "Parser.h"
#include "CommandRegistry.h"
#include "CommandFactories.h"
#include "RenderCommand.h"
#include "CommandHistory.h"
#include "../Model/Model.h"
#include "../Viewer/View.h"
#include <memory>

namespace Controller {
    class Controller {
    private:
        Parser parser_;

        Controller() {
            registerAllCommands();
        }

        Controller(const Controller&);
        Controller& operator=(const Controller&);

    public:
        static Controller& getInstance() {
            static Controller instance;
            return instance;
        }
        void registerAllCommands() {
            auto& registry = CommandRegistry::getInstance();

            registry.registerCommand(std::unique_ptr<ICommandFactory>(new CreatePresentationFactory()));
            registry.registerCommand(std::unique_ptr<ICommandFactory>(new LoadPresentationFactory()));
            registry.registerCommand(std::unique_ptr<ICommandFactory>(new SavePresentationFactory()));
            registry.registerCommand(std::unique_ptr<ICommandFactory>(new AddSlideFactory()));
            registry.registerCommand(std::unique_ptr<ICommandFactory>(new RemoveSlideFactory()));
            registry.registerCommand(std::unique_ptr<ICommandFactory>(new DuplicateSlideFactory()));
            registry.registerCommand(std::unique_ptr<ICommandFactory>(new MoveSlideFactory()));
            registry.registerCommand(std::unique_ptr<ICommandFactory>(new AddShapeFactory()));
            registry.registerCommand(std::unique_ptr<ICommandFactory>(new AddTextFactory()));
            registry.registerCommand(std::unique_ptr<ICommandFactory>(new ShowFactory()));
            registry.registerCommand(std::unique_ptr<ICommandFactory>(new RenderFactory()));  // ? New!
            registry.registerCommand(std::unique_ptr<ICommandFactory>(new UndoFactory()));
            registry.registerCommand(std::unique_ptr<ICommandFactory>(new RedoFactory()));
            registry.registerCommand(std::unique_ptr<ICommandFactory>(new HelpFactory()));
            registry.registerCommand(std::unique_ptr<ICommandFactory>(new ExitFactory()));
        }

        void processInput(const std::string& input) {
            auto& view = View::ViewFacade::getInstance();
            auto& history = CommandHistory::getInstance();
            Parser::ParseResult parseResult = parser_.parse(input);

            if (!parseResult.syntaxValid) {
                view.showError("Syntax error: " + parseResult.errorMessage);
                return;
            }

            if (parseResult.tokens.empty()) {
                return;
            }
            const std::string& commandName = parseResult.tokens[0];
            auto& registry = CommandRegistry::getInstance();
            ICommandFactory* factory = registry.getFactory(commandName);

            if (factory == 0) {
                view.showWarning("Unknown command: " + commandName);
                return;
            }
            try {
                std::unique_ptr<ICommand> command = factory->createCommand(parseResult.tokens);

                if (command.get()) {
                    if (commandName == "undo" || commandName == "redo" ||
                        commandName == "help" || commandName == "show" ||
                        commandName == "exit" || commandName == "create_presentation" ||
                        commandName == "load_presentation" || commandName == "save_presentation" ||
                        commandName == "render") {  // ? Render is not undoable
                        // Non-undoable commands execute directly
                        command->execute();
                    }
                    else {
                        history.executeCommand(std::move(command));
                    }
                }
            }
            catch (const std::exception& e) {
                view.showError("Command execution failed: " + std::string(e.what()));
            }
        }
    };

} 
