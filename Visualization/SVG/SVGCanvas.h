#pragma once
#include "../../Visualization/ICanvas.h"
#include "../../Visualization/SVG/SVGBrush.h"
#include <sstream>
#include <string>

namespace Visualization {
    namespace SVG {
        class SVGCanvas : public ICanvas {
        private:
            int width_;
            int height_;
            std::ostringstream content_;
            bool isDrawing_;

        public:
            explicit SVGCanvas(int width = 800, int height = 600)
                : width_(width), height_(height), isDrawing_(false) {
            }

            void beginDrawing() {
                content_.str("");  // Clear previous content
                content_ << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
                content_ << "<svg xmlns=\"http://www.w3.org/2000/svg\" ";
                content_ << "width=\"" << width_ << "\" height=\"" << height_ << "\">\n";
                isDrawing_ = true;
            }

            void endDrawing() {
                content_ << "</svg>\n";
                isDrawing_ = false;
            }

            void drawRectangle(int x, int y, int width, int height, const IBrush& brush) {
                if (!isDrawing_) return;

                const SVGBrush* svgBrush = dynamic_cast<const SVGBrush*>(&brush);
                std::string style = svgBrush ? svgBrush->toSVGStyle() : getDefaultStyle(brush);

                content_ << "  <rect x=\"" << x << "\" y=\"" << y
                    << "\" width=\"" << width << "\" height=\"" << height
                    << "\" style=\"" << style << "\"/>\n";
            }

            void drawCircle(int centerX, int centerY, int radius, const IBrush& brush) {
                if (!isDrawing_) return;

                const SVGBrush* svgBrush = dynamic_cast<const SVGBrush*>(&brush);
                std::string style = svgBrush ? svgBrush->toSVGStyle() : getDefaultStyle(brush);

                content_ << "  <circle cx=\"" << centerX << "\" cy=\"" << centerY
                    << "\" r=\"" << radius
                    << "\" style=\"" << style << "\"/>\n";
            }

            void drawEllipse(int centerX, int centerY, int radiusX, int radiusY, const IBrush& brush) {
                if (!isDrawing_) return;

                const SVGBrush* svgBrush = dynamic_cast<const SVGBrush*>(&brush);
                std::string style = svgBrush ? svgBrush->toSVGStyle() : getDefaultStyle(brush);

                content_ << "  <ellipse cx=\"" << centerX << "\" cy=\"" << centerY
                    << "\" rx=\"" << radiusX << "\" ry=\"" << radiusY
                    << "\" style=\"" << style << "\"/>\n";
            }

            void drawLine(int x1, int y1, int x2, int y2, const IBrush& brush) {
                if (!isDrawing_) return;

                const SVGBrush* svgBrush = dynamic_cast<const SVGBrush*>(&brush);
                std::string style = svgBrush ? svgBrush->toSVGStyle() : getDefaultStyle(brush);

                content_ << "  <line x1=\"" << x1 << "\" y1=\"" << y1
                    << "\" x2=\"" << x2 << "\" y2=\"" << y2
                    << "\" style=\"" << style << "\"/>\n";
            }

            void drawPolygon(const int* xPoints, const int* yPoints, int numPoints, const IBrush& brush) {
                if (!isDrawing_ || numPoints < 3) return;

                const SVGBrush* svgBrush = dynamic_cast<const SVGBrush*>(&brush);
                std::string style = svgBrush ? svgBrush->toSVGStyle() : getDefaultStyle(brush);

                content_ << "  <polygon points=\"";
                for (int i = 0; i < numPoints; ++i) {
                    if (i > 0) content_ << " ";
                    content_ << xPoints[i] << "," << yPoints[i];
                }
                content_ << "\" style=\"" << style << "\"/>\n";
            }

            void drawText(int x, int y, const std::string& text, const IBrush& brush) {
                if (!isDrawing_) return;

                const SVGBrush* svgBrush = dynamic_cast<const SVGBrush*>(&brush);

                content_ << "  <text x=\"" << x << "\" y=\"" << y << "\" ";
                content_ << "font-family=\"" << brush.getFontFamily() << "\" ";
                content_ << "font-size=\"" << brush.getFontSize() << "\" ";

                if (svgBrush) {
                    content_ << "style=\"" << svgBrush->toSVGStyle() << "\"";
                }
                else {
                    content_ << "fill=\"" << brush.getFillColor() << "\"";
                }

                content_ << ">" << escapeXML(text) << "</text>\n";
            }

            int getWidth() const { return width_; }
            int getHeight() const { return height_; }

            std::string getOutput() const {
                return content_.str();
            }

        private:
            std::string getDefaultStyle(const IBrush& brush) const {
                std::string style = "stroke:" + brush.getStrokeColor() + ";";
                style += "fill:" + brush.getFillColor() + ";";
                style += "stroke-width:" + std::to_string(brush.getStrokeWidth()) + ";";
                style += "opacity:" + std::to_string(brush.getOpacity()) + ";";
                return style;
            }

            std::string escapeXML(const std::string& text) const {
                std::string result;
                for (size_t i = 0; i < text.length(); ++i) {
                    char c = text[i];
                    switch (c) {
                    case '&':  result += "&amp;"; break;
                    case '<':  result += "&lt;"; break;
                    case '>':  result += "&gt;"; break;
                    case '"':  result += "&quot;"; break;
                    case '\'': result += "&apos;"; break;
                    default:   result += c; break;
                    }
                }
                return result;
            }
        };

    } // namespace SVG
} // namespace Visualization
