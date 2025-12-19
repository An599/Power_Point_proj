
//#pragma once
//#include <string>
//#include <iostream>
//#include <vector>
//#include <utility>
//
//namespace View {
//
//    class ViewFacade {
//    private:
//        ViewFacade() {}
//        ViewFacade(const ViewFacade&);
//        ViewFacade& operator=(const ViewFacade&);
//
//    public:
//        static ViewFacade& getInstance() {
//            static ViewFacade instance;
//            return instance;
//        }
//
//        void showError(const std::string& message) const {
//            std::cerr << "[ERROR] " << message << "\n";
//        }
//
//        void showInfo(const std::string& message) const {
//            std::cout << "[INFO] " << message << "\n";
//        }
//
//        void showSuccess(const std::string& message) const {
//            std::cout << "[SUCCESS] " << message << "\n";
//        }
//
//        void showWarning(const std::string& message) const {
//            std::cout << "[WARNING] " << message << "\n";
//        }
//
//        void showPrompt() const {
//            std::cout << "> ";
//            std::cout.flush();
//        }
//
//        void showHelp() const {
//            std::cout << "\n=== Available Commands ===\n\n";
//
//            std::cout << "PRESENTATION:\n";
//            std::cout << "  create_presentation <title>             - Create a new presentation\n";
//            std::cout << "  load_presentation -path <filepath>      - Load presentation from file\n";
//            std::cout << "  save_presentation <filename>            - Save current presentation\n\n";
//
//            std::cout << "SLIDES:\n";
//            std::cout << "  add_slide [title] [position]            - Add slide\n";
//            std::cout << "    Examples:\n";
//            std::cout << "      add_slide                           - Add slide at end with default title\n";
//            std::cout << "      add_slide MySlide                   - Add slide 'MySlide' at end\n";
//            std::cout << "      add_slide MySlide 0                 - Add slide 'MySlide' at position 0\n";
//            std::cout << "      add_slide 2                         - Add default slide at position 2\n";
//            std::cout << "  remove_slide <position>                 - Remove slide at position (0-indexed)\n\n";
//
//            std::cout << "SHAPES:\n";
//            std::cout << "  add_shape <type> [options]              - Add shape to slide\n";
//            std::cout << "    Shape types: rectangle, circle, triangle, trapezoid, parallelogram, rhombus\n";
//            std::cout << "    Options:\n";
//            std::cout << "      -coord <x> <y>                      - Position (default: 0,0)\n";
//            std::cout << "      -size <width> <height>              - Size (default: 100x100)\n";
//            std::cout << "      -color <color>                      - Color (default: black)\n";
//            std::cout << "      -text <text>                        - Text label\n";
//            std::cout << "      -slide <index>                      - Target slide (default: 0)\n";
//            std::cout << "    Example:\n";
//            std::cout << "      add_shape rectangle -coord 50 50 -size 200 100 -color blue -slide 0\n\n";
//
//            std::cout << "RENDERING:\n";
//            std::cout << "  render <output.svg> [-slide <index>]   - Render to SVG file\n";
//            std::cout << "    Examples:\n";
//            std::cout << "      render out.svg                      - Render first slide\n";
//            std::cout << "      render out.svg -slide 0             - Render slide 0\n";
//            std::cout << "      render out.svg -slide 1             - Render slide 1\n";
//            std::cout << "  show                                    - Display presentation in console\n\n";
//
//            std::cout << "HISTORY:\n";
//            std::cout << "  undo                                    - Undo last operation\n";
//            std::cout << "  redo                                    - Redo undone operation\n\n";
//
//            std::cout << "OTHER:\n";
//            std::cout << "  help                                    - Show this help\n";
//            std::cout << "  exit                                    - Exit program\n";
//            std::cout << "\n";
//        }
//
//        void showPresentation(const std::string& title,
//            const std::vector<std::pair<std::string, std::vector<std::string> > >& slides) const {
//            std::cout << "\n=== Presentation: " << title << " ===\n";
//            if (slides.empty()) {
//                std::cout << "(No slides)\n";
//                return;
//            }
//
//            for (size_t i = 0; i < slides.size(); ++i) {
//                std::cout << "\n[Slide " << i << ": " << slides[i].first << "]\n";
//                if (slides[i].second.empty()) {
//                    std::cout << "  (No shapes)\n";
//                }
//                else {
//                    for (size_t j = 0; j < slides[i].second.size(); ++j) {
//                        std::cout << "  " << slides[i].second[j] << "\n";
//                    }
//                }
//            }
//            std::cout << "\n";
//        }
//
//        void showWelcome() const {
//            std::cout << "=================================\n";
//            std::cout << "   Mini PowerPoint CLI Program\n";
//            std::cout << "   With Undo/Redo & Visualization\n";
//            std::cout << "=================================\n";
//            std::cout << "Type 'help' for a list of commands.\n\n";
//        }
//    };
//
//} // namespace View

#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <utility>

namespace View {

    class ViewFacade {
    private:
        ViewFacade() {}
        ViewFacade(const ViewFacade&);
        ViewFacade& operator=(const ViewFacade&);

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
            std::cout << "\n=== Available Commands ===\n\n";

            std::cout << "PRESENTATION:\n";
            std::cout << "  create_presentation <title>             - Create a new presentation\n";
            std::cout << "  load_presentation -path <file.json>     - Load from JSON file\n";
            std::cout << "  save_presentation <file.json>           - Save to JSON file\n\n";

            std::cout << "SLIDES:\n";
            std::cout << "  add_slide [position]                    - Add slide (optionally at position)\n";
            std::cout << "  remove_slide <position>                 - Remove slide (0-indexed)\n";
            std::cout << "  duplicate_slide <position>              - Duplicate slide at position\n";
            std::cout << "  move_slide <from_position> <to_position> - Move slide from one position to another\n\n";

            std::cout << "SHAPES (with Z-order layering):\n";
            std::cout << "  add_shape <type> [options]              - Add shape to slide\n";
            std::cout << "    Shape types: rectangle, circle, triangle, trapezoid, parallelogram, rhombus\n";
            std::cout << "    Options:\n";
            std::cout << "      -coord <x> <y>                      - Position (default: 0,0)\n";
            std::cout << "      -size <width> <height>              - Size (default: 100x100)\n";
            std::cout << "      -color <color>                      - Stroke color (default: black)\n";
            std::cout << "      -fill <color>                       - Fill color (default: none)\n";
            std::cout << "      -text <text>                        - Text label\n";
            std::cout << "      -slide <index>                      - Target slide (default: 0)\n";
            std::cout << "      -front                              - Place shape on top layer\n";
            std::cout << "    Example:\n";
            std::cout << "      add_shape circle -coord 50 50 -size 100 100 -color blue -fill red -front\n";
            std::cout << "  add_text [options]                      - Add text (invisible rectangle with text)\n";
            std::cout << "    Options:\n";
            std::cout << "      -text <text>                        - Text content (required)\n";
            std::cout << "      -coord <x> <y>                      - Position (default: 0,0)\n";
            std::cout << "      -size <width> <height>              - Size (default: 100x50)\n";
            std::cout << "      -color <color>                      - Text color (default: black)\n";
            std::cout << "      -slide <index>                      - Target slide (default: 0)\n";
            std::cout << "      -front                              - Place on top layer\n";
            std::cout << "    Example:\n";
            std::cout << "      add_text -text \"Hello\" -coord 50 50 -size 200 50 -color blue\n\n";

            std::cout << "RENDERING:\n";
            std::cout << "  render <output.svg> [-slide <index>]   - Render to SVG (respects Z-order)\n";
            std::cout << "  show                                    - Display in console (sorted by Z-order)\n\n";

            std::cout << "HISTORY:\n";
            std::cout << "  undo                                    - Undo last operation\n";
            std::cout << "  redo                                    - Redo undone operation\n\n";

            std::cout << "OTHER:\n";
            std::cout << "  help                                    - Show this help\n";
            std::cout << "  exit                                    - Exit program\n\n";

            std::cout << "NOTE: Shapes are rendered back-to-front based on Z-order.\n";
            std::cout << "      Use -front flag to place a shape on top of others.\n";
            std::cout << "      Files are saved/loaded in JSON format.\n\n";
        }

        void showPresentation(const std::string& title,
            const std::vector<std::pair<std::string, std::vector<std::string>>>& slides) const {
            std::cout << "\n=== Presentation: " << title << " ===\n";
            if (slides.empty()) {
                std::cout << "(No slides)\n";
                return;
            }

            for (size_t i = 0; i < slides.size(); ++i) {
                std::cout << "\n[Slide " << i << "]\n";
                if (slides[i].second.empty()) {
                    std::cout << "  (No shapes)\n";
                }
                else {
                    std::cout << "  Shapes (in Z-order, back to front):\n";
                    for (size_t j = 0; j < slides[i].second.size(); ++j) {
                        std::cout << "  " << (j + 1) << ". " << slides[i].second[j] << "\n";
                    }
                }
            }
            std::cout << "\n";
        }

        void showWelcome() const {
            std::cout << "===========================================\n";
            std::cout << "   Mini PowerPoint CLI with Z-Order\n";
            std::cout << "   Visitor Pattern | JSON Serialization\n";
            std::cout << "===========================================\n";
            std::cout << "Type 'help' for a list of commands.\n\n";
        }
    };

} // namespace View