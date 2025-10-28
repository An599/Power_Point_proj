#pragma once
#include "Controller/Parser.h"
#include "Controller/CommandRegistry.h"
#include "Controller/CommandFactories.h"
#include "Model/Model.h"
#include "Viewer/View.h"
#include <memory>

namespace Controller {
    class Controller {
    private:
        Parser parser_;

        Controller() {
            registerAllCommands();
        }

        Controller(const Controller&) = delete;
        Controller& operator=(const Controller&) = delete;

    public:
        static Controller& getInstance() {
            static Controller instance;
            return instance;
        }

        void registerAllCommands() {
            auto& registry = CommandRegistry::getInstance();

            registry.registerCommand(std::make_unique<CreatePresentationFactory>());
            registry.registerCommand(std::make_unique<LoadPresentationFactory>());
            registry.registerCommand(std::make_unique<SavePresentationFactory>());
            registry.registerCommand(std::make_unique<AddSlideFactory>());
            registry.registerCommand(std::make_unique<RemoveSlideFactory>());
            registry.registerCommand(std::make_unique<AddShapeFactory>());
            registry.registerCommand(std::make_unique<ShowFactory>());
            registry.registerCommand(std::make_unique<HelpFactory>());
            registry.registerCommand(std::make_unique<ExitFactory>());
        }

        void processInput(const std::string& input) {
            auto& view = View::ViewFacade::getInstance();

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

            if (factory == nullptr) {
                view.showWarning("Unknown command: " + commandName);
                return;
            }
            try {
                std::unique_ptr<ICommand> command = factory->createCommand(parseResult.tokens);

                if (command) {
                    command->execute();
                }
            }
            catch (const std::exception& e) {
                view.showError("Command execution failed: " + std::string(e.what()));
            }
        }
    };

} 
