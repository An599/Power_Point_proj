#pragma once
#include "IVisualization.h"
#include "../Painting/SVGCanvas.h"
#include "../Painting/IPainter.h"
#include "../Painting/SVGPainter.h"
#include "../Painting/Pen.h"
#include "../Painting/Brush.h"
#include "../Painting/TransformedPainter.h"
#include <sstream>

namespace Visualization {

    // SvgVisualization - SVG implementation of IVisualization
    // Generates SVG representation of all slides in the presentation
    class SvgVisualization : public IVisualization {
    public:
        std::string visualize(const Model::Presentation& presentation) const override {
            if (presentation.slideCount() == 0) {
                return generateEmptySvg();
            }

            // Calculate canvas dimensions
            int slideHeight = 250;
            int spacing = 20;
            int titleHeight = 80;
            int canvasHeight = titleHeight + static_cast<int>(presentation.slideCount() * (slideHeight + spacing));
            if (canvasHeight < 600) canvasHeight = 600;
            int canvasWidth = 800;

            // Create SVG canvas
            Painting::SVGCanvas canvas(canvasWidth, canvasHeight);
            Painting::SVGPainter& painter = canvas.getPainter();

            canvas.beginDrawing();

            // Left margin (tab = 4 spaces ≈ 60 pixels)
            const int leftMargin = 60;
            
            // Draw presentation title with proper left margin
            painter.drawTextLeft(leftMargin, 40, presentation.title(), "Arial", 24, "black");
            
            // Draw separator line below title (with margins)
            Painting::Pen titleSeparatorPen("gray", 2, Painting::Pen::Type::SOLID);
            painter.drawLine(leftMargin, 60, canvasWidth - leftMargin, 60, titleSeparatorPen);

            // Render each slide with more spacing
            int yOffset = titleHeight + 10;
            for (size_t i = 0; i < presentation.slideCount(); ++i) {
                const Model::Slide* slide = presentation.getSlide(i);

                // Draw slide separator (with margins)
                if (i > 0) {
                    Painting::Pen separatorPen("lightgray", 1, Painting::Pen::Type::DASHED);
                    painter.drawLine(leftMargin, yOffset - spacing / 2, canvasWidth - leftMargin, yOffset - spacing / 2, separatorPen);
                }

                    // Draw slide number (left-aligned with proper margin)
                    std::string slideTitle = "Slide " + std::to_string(static_cast<long long>(i));
                    painter.drawTextLeft(leftMargin, yOffset, slideTitle, "Arial", 18, "blue");
                yOffset += 30;

                // Draw slide background (using polygon since IPainter doesn't have drawRectangle)
                // Slide starts at leftMargin to maintain proper indentation
                int slideX = leftMargin;
                int slideY = yOffset;
                int slideWidth = canvasWidth - 2 * leftMargin;
                int slideHeightArea = slideHeight - 30;
                Painting::Pen bgPen("lightgray", 1, Painting::Pen::Type::SOLID);
                Painting::Brush bgBrush("white", Painting::Brush::Style::SOLID);
                int bgXPoints[4] = { slideX, slideX + slideWidth, slideX + slideWidth, slideX };
                int bgYPoints[4] = { slideY, slideY, slideY + slideHeightArea, slideY + slideHeightArea };
                painter.drawPolygon(bgXPoints, bgYPoints, 4, bgPen, bgBrush);

                // Add padding inside slide area to prevent text/shapes from being cut off
                // Use larger padding to account for text-anchor="middle" which extends text on both sides
                int padding = 20;
                int contentX = slideX + padding;
                int contentY = slideY + padding;
                int contentWidth = slideWidth - 2 * padding;
                int contentHeight = slideHeightArea - 2 * padding;

                // Draw shapes with offset into slide area (with padding)
                const auto& shapes = slide->getShapes();
                if (shapes.empty()) {
                    // Use left-aligned text for empty slide message
                    painter.drawTextLeft(
                        slideX + padding, slideY + slideHeightArea / 2, 
                        "(No shapes on this slide)", "Arial", 12, "gray");
                }
                else {
                    // Create transformed painter that offsets shapes into slide area with padding
                    Painting::TransformedPainter transformedPainter(painter, contentX, contentY);
                    
                    // Draw each shape (in order they appear in the vector, which represents Z-order)
                    for (const auto& shape : shapes) {
                        if (shape) {
                            shape->draw(transformedPainter);
                        }
                    }
                }

                yOffset += slideHeight;
            }

            canvas.endDrawing();

            // Return SVG string
            return canvas.getOutput();
        }

    private:
        std::string generateEmptySvg() const {
            std::ostringstream oss;
            oss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
            oss << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"800\" height=\"200\">\n";
            oss << "  <text x=\"400\" y=\"100\" text-anchor=\"middle\" font-family=\"Arial\" font-size=\"18\" fill=\"gray\">\n";
            oss << "    Empty Presentation\n";
            oss << "  </text>\n";
            oss << "</svg>\n";
            return oss.str();
        }
    };

} // namespace Visualization

