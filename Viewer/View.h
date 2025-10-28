#pragma once
#include <string>
#include <iostream>
#include <vector>

namespace View {

    // View Facade - doesn't know about Model or Controller
    class ViewFacade {
    private:
        ViewFacade() = default;
        ViewFacade(const ViewFacade&) = delete;
        ViewFacade& operator=(const ViewFacade&) = delete;

    public:
        static ViewFacade& getInstance() {
            static ViewFacade instance;
            return instance;
        }

        void showError(const std::string& message) const {
            std::cerr << "[ERROR] " << message << "\n";
        }

        void showInfo(const std::string& message) const {
            std::cout << "[INFO] " << message << "\n";
        }

        void showSuccess(const std::string& message) const {
            std::cout << "[SUCCESS] " << message << "\n";
        }

        void showWarning(const std::string& message) const {
            std::cout << "[WARNING] " << message << "\n";
        }

        void showPrompt() const {
            std::cout << "> ";
            std::cout.flush();
        }

        void showHelp() const {
            std::cout << "\n=== Available Commands ===\n";
            std::cout << "  create_presentation <title>             - Create a new presentation\n";
            std::cout << "  load_presentation -path <filepath>      - Load presentation from file\n";
            std::cout << "  save_presentation <filename>            - Save current presentation\n";
            std::cout << "  add_slide [title] [position]            - Add slide (at position if given)\n";
            std::cout << "  remove_slide <position>                 - Remove slide at position\n";
            std::cout << "  add_shape <type> [options]              - Add shape to slide\n";
            std::cout << "    Options:\n";
            std::cout << "      -coord <x> <y>                      - Position (default: 0,0)\n";
            std::cout << "      -size <width> <height>              - Size (default: 100x100)\n";
            std::cout << "      -color <color>                      - Color (default: black)\n";
            std::cout << "      -text <text>                        - Text content\n";
            std::cout << "      -slide <index>                      - Slide index (default: 0)\n";
            std::cout << "    Shape types: rectangle, circle, triangle\n";
            std::cout << "  show                                    - Display presentation\n";
            std::cout << "  help                                    - Show this help\n";
            std::cout << "  exit                                    - Exit program\n";
            std::cout << "\n";
        }

        void showPresentation(const std::string& title,
            const std::vector<std::pair<std::string, std::vector<std::string>>>& slides) const {
            std::cout << "\n=== Presentation: " << title << " ===\n";
            if (slides.empty()) {
                std::cout << "(No slides)\n";
                return;
            }

            for (size_t i = 0; i < slides.size(); ++i) {
                std::cout << "\n[Slide " << i << ": " << slides[i].first << "]\n";
                if (slides[i].second.empty()) {
                    std::cout << "  (No shapes)\n";
                }
                else {
                    for (const auto& shape : slides[i].second) {
                        std::cout << "  " << shape << "\n";
                    }
                }
            }
            std::cout << "\n";
        }

        void showWelcome() const {
            std::cout << "=================================\n";
            std::cout << "   Mini PowerPoint CLI Program\n";
            std::cout << "=================================\n";
            std::cout << "Type 'help' for a list of commands.\n\n";
        }
    };

} // namespace View
