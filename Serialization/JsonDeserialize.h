#pragma once
#include "IDeserialize.h"
#include "../Model/Shapes.h"
#include "../Model/TextShape.h"
#include <fstream>
#include <sstream>

namespace Serialization {

    class JsonDeserialize : public IDeserialize {
    public:
        std::unique_ptr<Model::Presentation> load(const std::string& filename) const override {
            std::ifstream in(filename);
            if (!in) {
                throw std::runtime_error("Cannot open file for reading: " + filename);
            }

            std::string content((std::istreambuf_iterator<char>(in)),
                               std::istreambuf_iterator<char>());

            std::string title = extractString(content, "\"title\"");
            auto presentation = std::make_unique<Model::Presentation>(title);

            size_t slidesStart = content.find("\"slides\"");
            if (slidesStart != std::string::npos) {
                parseSlides(content, slidesStart, *presentation);
            }

            return presentation;
        }

    private:
        std::string extractString(const std::string& json, const std::string& key) const {
            size_t pos = json.find(key);
            if (pos == std::string::npos) return "";

            pos = json.find(':', pos);
            if (pos == std::string::npos) return "";

            pos = json.find('"', pos);
            if (pos == std::string::npos) return "";
            pos++;

            size_t end = pos;
            while (end < json.length() && (json[end] != '"' || (end > 0 && json[end-1] == '\\'))) {
                if (json[end] == '\\' && end + 1 < json.length()) {
                    end += 2;
                } else {
                    end++;
                }
            }

            std::string result = json.substr(pos, end - pos);

            std::ostringstream oss;
            for (size_t i = 0; i < result.length(); ++i) {
                if (result[i] == '\\' && i + 1 < result.length()) {
                    switch (result[i + 1]) {
                    case '"': oss << '"'; i++; break;
                    case '\\': oss << '\\'; i++; break;
                    case 'n': oss << '\n'; i++; break;
                    case 'r': oss << '\r'; i++; break;
                    case 't': oss << '\t'; i++; break;
                    default: oss << result[i]; break;
                    }
                } else {
                    oss << result[i];
                }
            }
            return oss.str();
        }

        void parseSlides(const std::string& json, size_t startPos, Model::Presentation& presentation) const {

            size_t bracketPos = json.find('[', startPos);
            if (bracketPos == std::string::npos) return;

            size_t pos = bracketPos + 1;
            int depth = 1;
            size_t slideStart = pos;

            while (pos < json.length() && depth > 0) {
                if (json[pos] == '[' || json[pos] == '{') depth++;
                else if (json[pos] == ']' || json[pos] == '}') depth--;

                if (depth == 1 && json[pos] == '}') {

                    std::string slideJson = json.substr(slideStart, pos - slideStart + 1);
                    parseSlide(slideJson, presentation);
                    slideStart = pos + 1;

                    while (slideStart < json.length() && (json[slideStart] == ',' || json[slideStart] == ' ' || json[slideStart] == '\n')) {
                        slideStart++;
                    }
                }
                pos++;
            }
        }

        void parseSlide(const std::string& slideJson, Model::Presentation& presentation) const {
            auto slide = std::make_unique<Model::Slide>();

            size_t shapesStart = slideJson.find("\"shapes\"");
            if (shapesStart != std::string::npos) {
                parseShapes(slideJson, shapesStart, *slide);
            }

            presentation.addSlide(std::move(slide));
        }

        void parseShapes(const std::string& json, size_t startPos, Model::Slide& slide) const {
            size_t bracketPos = json.find('[', startPos);
            if (bracketPos == std::string::npos) return;

            size_t pos = bracketPos + 1;
            int depth = 1;
            size_t shapeStart = pos;

            while (pos < json.length() && depth > 0) {
                if (json[pos] == '[' || json[pos] == '{') depth++;
                else if (json[pos] == ']' || json[pos] == '}') depth--;

                if (depth == 1 && json[pos] == '}') {

                    std::string shapeJson = json.substr(shapeStart, pos - shapeStart + 1);
                    parseShape(shapeJson, slide);
                    shapeStart = pos + 1;

                    while (shapeStart < json.length() && (json[shapeStart] == ',' || json[shapeStart] == ' ' || json[shapeStart] == '\n')) {
                        shapeStart++;
                    }
                }
                pos++;
            }
        }

        void parseShape(const std::string& shapeJson, Model::Slide& slide) const {
            std::string type = extractString(shapeJson, "\"type\"");
            int x = extractInt(shapeJson, "\"x\"");
            int y = extractInt(shapeJson, "\"y\"");
            int width = extractInt(shapeJson, "\"width\"");
            int height = extractInt(shapeJson, "\"height\"");
            std::string color = extractString(shapeJson, "\"color\"");
            std::string fillColor = extractString(shapeJson, "\"fillColor\"");
            if (fillColor.empty()) fillColor = "none";
            std::string text = extractString(shapeJson, "\"text\"");

            Model::BoundingBox bounds(x, y, width, height);
            std::unique_ptr<Model::IShape> shape;

            if (type == "Rectangle") {
                shape = std::make_unique<Model::Rectangle>(bounds, color, fillColor, text);
            }
            else if (type == "Circle") {
                shape = std::make_unique<Model::Circle>(bounds, color, fillColor, text);
            }
            else if (type == "Triangle") {
                shape = std::make_unique<Model::Triangle>(bounds, color, fillColor, text);
            }
            else if (type == "Trapezoid") {
                shape = std::make_unique<Model::Trapezoid>(bounds, color, fillColor, text);
            }
            else if (type == "Parallelogram") {
                shape = std::make_unique<Model::Parallelogram>(bounds, color, fillColor, text);
            }
            else if (type == "Rhombus") {
                shape = std::make_unique<Model::Rhombus>(bounds, color, fillColor, text);
            }
            else if (type == "Text") {
                shape = std::make_unique<Model::TextShape>(bounds, text, color);
            }

            if (shape) {
                slide.addShape(std::move(shape));
            }
        }

        int extractInt(const std::string& json, const std::string& key) const {
            size_t pos = json.find(key);
            if (pos == std::string::npos) return 0;

            pos = json.find(':', pos);
            if (pos == std::string::npos) return 0;

            while (pos < json.length() && (json[pos] == ' ' || json[pos] == ':')) pos++;

            size_t end = pos;
            while (end < json.length() && (std::isdigit(json[end]) || json[end] == '-')) end++;

            std::string numStr = json.substr(pos, end - pos);
            return std::stoi(numStr);
        }
    };

}
