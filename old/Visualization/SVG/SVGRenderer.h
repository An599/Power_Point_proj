#pragma once
#include "../../Visualization/IRenderer.h"
#include "../../Visualization/ICanvas.h"
#include "../../Visualization/IBrush.h"
#include "../../Visualization/SVG/SVGBrush.h"
#include "../../Model/IShape.h"
#include "../../Model/Slide.h"
#include "../../Model/Presentation.h"
#include "../../Model/BoundingBox.h"
#include <memory>

namespace Visualization {
    namespace SVG {

        // SVGRenderer - Concrete renderer for SVG output
        // Cohesive - translates Model shapes to Canvas operations
        // Decoupled - works through interfaces (ICanvas, IBrush)
        // Follows Strategy pattern for rendering
        class SVGRenderer : public IRenderer {
        public:
            void renderShape(const Model::IShape& shape, ICanvas& canvas, const IBrush& brush) {
                // Get bounding box (primitive abstraction)
                Model::BoundingBox bbox = shape.getBoundingBox();

                // Determine shape type and render accordingly
                std::string type = shape.getType();

                if (type == "Rectangle") {
                    canvas.drawRectangle(bbox.getX(), bbox.getY(),
                        bbox.getWidth(), bbox.getHeight(), brush);
                }
                else if (type == "Circle") {
                    // Circle uses bounding box - draw as ellipse
                    int centerX = bbox.getCenterX();
                    int centerY = bbox.getCenterY();
                    int radiusX = bbox.getWidth() / 2;
                    int radiusY = bbox.getHeight() / 2;
                    canvas.drawEllipse(centerX, centerY, radiusX, radiusY, brush);
                }
                else if (type == "Triangle") {
                    // Triangle from bounding box - isosceles triangle
                    int x1 = bbox.getX();
                    int y1 = bbox.getY() + bbox.getHeight();  // Bottom left
                    int x2 = bbox.getX() + bbox.getWidth();
                    int y2 = bbox.getY() + bbox.getHeight();  // Bottom right
                    int x3 = bbox.getCenterX();
                    int y3 = bbox.getY();                     // Top center

                    int xPoints[] = { x1, x2, x3 };
                    int yPoints[] = { y1, y2, y3 };
                    canvas.drawPolygon(xPoints, yPoints, 3, brush);
                }

                // Draw text if shape has text
                std::string text = shape.getText();
                if (!text.empty()) {
                    // Create text brush with shape's color
                    SVGBrush textBrush("none", shape.getColor(), 1, 1.0f, "solid", "Arial", 14);
                    canvas.drawText(bbox.getCenterX(), bbox.getCenterY(), text, textBrush);
                }
            }

            void renderSlide(const Model::Slide& slide, ICanvas& canvas) {
                // Draw slide background
                SVGBrush bgBrush("gray", "white", 2);
                canvas.drawRectangle(0, 0, canvas.getWidth(), canvas.getHeight(), bgBrush);

                // No slide title needed

                // Draw all shapes on slide
                const std::vector<std::unique_ptr<Model::IShape> >& shapes = slide.getShapes();
                for (size_t i = 0; i < shapes.size(); ++i) {
                    const Model::IShape* shape = shapes[i].get();

                    // Create brush from shape's color
                    std::string color = shape->getColor();
                    SVGBrush shapeBrush(color, "none", 2);

                    renderShape(*shape, canvas, shapeBrush);
                }
            }

            void renderPresentation(const Model::Presentation& presentation, ICanvas& canvas) {
                // For now, render first slide only (or create multi-page SVG)
                if (presentation.slideCount() > 0) {
                    const Model::Slide* slide = presentation.getSlide(0);
                    renderSlide(*slide, canvas);
                }
            }

            IBrush* createBrush(const std::string& strokeColor,
                const std::string& fillColor,
                int strokeWidth) const {
                return new SVGBrush(strokeColor, fillColor, strokeWidth);
            }
        };

    } // namespace SVG
} // namespace Visualization