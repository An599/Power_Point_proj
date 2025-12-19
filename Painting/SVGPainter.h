#pragma once
#include "Painting/IPainter.h"
#include "Painting/Pen.h"
#include "Painting/Brush.h"
#include <sstream>
#include <string>

namespace Painting {

    // SVGPainter - concrete implementation working with strings
    // Cohesive - manages SVG generation
    // Decoupled - implements IPainter interface
    class SVGPainter : public IPainter {
    private:
        int width_;
        int height_;
        std::ostringstream content_;
        bool isPainting_;

    public:
        SVGPainter(int width = 800, int height = 600)
            : width_(width), height_(height), isPainting_(false) {
        }

        void beginPaint() override {
            content_.str("");  // Clear
            content_ << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
            content_ << "<svg xmlns=\"http://www.w3.org/2000/svg\" ";
            content_ << "width=\"" << width_ << "\" height=\"" << height_ << "\" ";
            content_ << "style=\"overflow: visible;\">\n";  // Allow content to be visible even if slightly outside

            // Background
            content_ << "  <rect x=\"0\" y=\"0\" width=\"" << width_
                << "\" height=\"" << height_
                << "\" fill=\"white\" stroke=\"gray\" stroke-width=\"1\"/>\n";

            isPainting_ = true;
        }

        void endPaint() override {
            content_ << "</svg>\n";
            isPainting_ = false;
        }

        void drawLine(int x1, int y1, int x2, int y2, const Pen& pen) override {
            if (!isPainting_) return;

            content_ << "  <line x1=\"" << x1 << "\" y1=\"" << y1
                << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\" ";
            content_ << "stroke=\"" << pen.getColor() << "\" ";
            content_ << "stroke-width=\"" << pen.getWidth() << "\" ";

            if (pen.getType() == Pen::Type::DASHED) {
                content_ << "stroke-dasharray=\"5,5\" ";
            }
            else if (pen.getType() == Pen::Type::DOTTED) {
                content_ << "stroke-dasharray=\"2,2\" ";
            }

            content_ << "/>\n";
        }

        void drawEllipse(int centerX, int centerY, int radiusX, int radiusY,
            const Pen& pen, const Brush& brush) override {
            if (!isPainting_) return;

            content_ << "  <ellipse cx=\"" << centerX << "\" cy=\"" << centerY
                << "\" rx=\"" << radiusX << "\" ry=\"" << radiusY << "\" ";

            // Brush (fill)
            if (brush.getStyle() == Brush::Style::NONE) {
                content_ << "fill=\"none\" ";
            }
            else {
                content_ << "fill=\"" << brush.getColor() << "\" ";
                content_ << "fill-opacity=\"" << brush.getOpacity() << "\" ";
            }

            // Pen (stroke)
            content_ << "stroke=\"" << pen.getColor() << "\" ";
            content_ << "stroke-width=\"" << pen.getWidth() << "\" ";

            if (pen.getType() == Pen::Type::DASHED) {
                content_ << "stroke-dasharray=\"5,5\" ";
            }
            else if (pen.getType() == Pen::Type::DOTTED) {
                content_ << "stroke-dasharray=\"2,2\" ";
            }

            content_ << "/>\n";
        }

        void drawPolygon(const int* xPoints, const int* yPoints, int numPoints,
            const Pen& pen, const Brush& brush) override {
            if (!isPainting_ || numPoints < 3) return;

            content_ << "  <polygon points=\"";
            for (int i = 0; i < numPoints; ++i) {
                if (i > 0) content_ << " ";
                content_ << xPoints[i] << "," << yPoints[i];
            }
            content_ << "\" ";

            // Brush (fill)
            if (brush.getStyle() == Brush::Style::NONE) {
                content_ << "fill=\"none\" ";
            }
            else {
                content_ << "fill=\"" << brush.getColor() << "\" ";
                content_ << "fill-opacity=\"" << brush.getOpacity() << "\" ";
            }

            // Pen (stroke)
            content_ << "stroke=\"" << pen.getColor() << "\" ";
            content_ << "stroke-width=\"" << pen.getWidth() << "\" ";

            if (pen.getType() == Pen::Type::DASHED) {
                content_ << "stroke-dasharray=\"5,5\" ";
            }
            else if (pen.getType() == Pen::Type::DOTTED) {
                content_ << "stroke-dasharray=\"2,2\" ";
            }

            content_ << "/>\n";
        }

        void drawText(int x, int y, const std::string& text,
            const std::string& fontFamily, int fontSize,
            const std::string& color) override {
            if (!isPainting_) return;

            // Use text-anchor="middle" for horizontal centering and dominant-baseline="middle" for vertical centering
            // This is good for text inside shapes (centered)
            content_ << "  <text x=\"" << x << "\" y=\"" << y << "\" ";
            content_ << "text-anchor=\"middle\" dominant-baseline=\"middle\" ";
            content_ << "font-family=\"" << fontFamily << "\" ";
            content_ << "font-size=\"" << fontSize << "\" ";
            content_ << "fill=\"" << color << "\">";
            content_ << escapeXML(text);
            content_ << "</text>\n";
        }
        
        // Draw left-aligned text (for labels, titles, etc.)
        void drawTextLeft(int x, int y, const std::string& text,
            const std::string& fontFamily, int fontSize,
            const std::string& color) {
            if (!isPainting_) return;

            // Use text-anchor="start" for left alignment and dominant-baseline="middle" for vertical centering
            content_ << "  <text x=\"" << x << "\" y=\"" << y << "\" ";
            content_ << "text-anchor=\"start\" dominant-baseline=\"middle\" ";
            content_ << "font-family=\"" << fontFamily << "\" ";
            content_ << "font-size=\"" << fontSize << "\" ";
            content_ << "fill=\"" << color << "\">";
            content_ << escapeXML(text);
            content_ << "</text>\n";
        }

        int getWidth() const override { return width_; }
        int getHeight() const override { return height_; }

        // Get SVG output as string
        std::string getSVG() const {
            return content_.str();
        }

    private:
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

} // namespace Painting
