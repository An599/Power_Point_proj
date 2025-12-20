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
        class SVGRenderer : public IRenderer {
        public:
            void renderShape(const Model::IShape& shape, ICanvas& canvas, const IBrush& brush) {
                Model::BoundingBox bbox = shape.getBoundingBox();
                std::string type = shape.getType();

                if (type == "Rectangle") {
                    canvas.drawRectangle(bbox.getX(), bbox.getY(),
                        bbox.getWidth(), bbox.getHeight(), brush);
                }
                else if (type == "Circle") {
                    int centerX = bbox.getCenterX();
                    int centerY = bbox.getCenterY();
                    int radiusX = bbox.getWidth() / 2;
                    int radiusY = bbox.getHeight() / 2;
                    canvas.drawEllipse(centerX, centerY, radiusX, radiusY, brush);
                }
                else if (type == "Triangle") {
                    int x1 = bbox.getX();
                    int y1 = bbox.getY() + bbox.getHeight();  
                    int x2 = bbox.getX() + bbox.getWidth();
                    int y2 = bbox.getY() + bbox.getHeight();  
                    int x3 = bbox.getCenterX();
                    int y3 = bbox.getY();                     

                    int xPoints[] = { x1, x2, x3 };
                    int yPoints[] = { y1, y2, y3 };
                    canvas.drawPolygon(xPoints, yPoints, 3, brush);
                }

                std::string text = shape.getText();
                if (!text.empty()) {
                    SVGBrush textBrush("none", shape.getColor(), 1, 1.0f, "solid", "Arial", 14);
                    canvas.drawText(bbox.getCenterX(), bbox.getCenterY(), text, textBrush);
                }
            }

            void renderSlide(const Model::Slide& slide, ICanvas& canvas) {
                SVGBrush bgBrush("gray", "white", 2);
                canvas.drawRectangle(0, 0, canvas.getWidth(), canvas.getHeight(), bgBrush);
                const std::vector<std::unique_ptr<Model::IShape> >& shapes = slide.getShapes();
                for (size_t i = 0; i < shapes.size(); ++i) {
                    const Model::IShape* shape = shapes[i].get();
                    std::string color = shape->getColor();
                    SVGBrush shapeBrush(color, "none", 2);

                    renderShape(*shape, canvas, shapeBrush);
                }
            }

            void renderPresentation(const Model::Presentation& presentation, ICanvas& canvas) {
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
