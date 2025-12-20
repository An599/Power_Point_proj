#pragma once
#include "ISerialize.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace Serialization {

    class JsonSerialize : public ISerialize {
    public:
        void save(const Model::Presentation& presentation, const std::string& filename) const override {
            std::ofstream out(filename);
            if (!out) {
                throw std::runtime_error("Cannot open file for writing: " + filename);
            }

            out << "{\n";
            out << "  \"title\": \"" << escapeJson(presentation.title()) << "\",\n";
            out << "  \"slides\": [\n";

            size_t slideCount = presentation.slideCount();
            for (size_t i = 0; i < slideCount; ++i) {
                const Model::Slide* slide = presentation.getSlide(i);
                out << "    {\n";
                out << "      \"shapes\": [\n";

                const auto& shapes = slide->getShapes();
                for (size_t j = 0; j < shapes.size(); ++j) {
                    out << "        " << serializeShape(shapes[j].get());
                    if (j < shapes.size() - 1) {
                        out << ",";
                    }
                    out << "\n";
                }

                out << "      ]\n";
                out << "    }";
                if (i < slideCount - 1) {
                    out << ",";
                }
                out << "\n";
            }

            out << "  ]\n";
            out << "}\n";
        }

    private:
        std::string escapeJson(const std::string& str) const {
            std::string result;
            for (char c : str) {
                switch (c) {
                case '"': result += "\\\""; break;
                case '\\': result += "\\\\"; break;
                case '\n': result += "\\n"; break;
                case '\r': result += "\\r"; break;
                case '\t': result += "\\t"; break;
                default: result += c; break;
                }
            }
            return result;
        }

        std::string serializeShape(const Model::IShape* shape) const {
            if (!shape) return "null";

            std::ostringstream oss;
            Model::BoundingBox bounds = shape->getBoundingBox();

            oss << "{\n";
            oss << "          \"type\": \"" << shape->getType() << "\",\n";
            oss << "          \"x\": " << bounds.getX() << ",\n";
            oss << "          \"y\": " << bounds.getY() << ",\n";
            oss << "          \"width\": " << bounds.getWidth() << ",\n";
            oss << "          \"height\": " << bounds.getHeight() << ",\n";
            oss << "          \"color\": \"" << escapeJson(shape->getColor()) << "\",\n";
            oss << "          \"fillColor\": \"" << escapeJson(shape->getFillColor()) << "\"";

            std::string text = shape->getText();
            if (!text.empty()) {
                oss << ",\n          \"text\": \"" << escapeJson(text) << "\"";
            }

            oss << "\n        }";
            return oss.str();
        }
    };

}
