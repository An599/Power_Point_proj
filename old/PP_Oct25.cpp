#include "Controller/Controller.h"
#include "Viewer/View.h"
#include <iostream>
#include <string>

int main() {
    auto& controller = Controller::Controller::getInstance();
    auto& view = View::ViewFacade::getInstance();

    view.showWelcome();

    std::string line;
    while (true) {
        view.showPrompt();

        if (!std::getline(std::cin, line)) {
            break;
        }

        if (line.empty()) {
            continue;
        }

        controller.processInput(line);
    }

    return 0;
}