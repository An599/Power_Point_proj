#pragma once
#include "../VisualizationFactory.h"
#include "SVGCanvas.h"
#include "SVGRenderer.h"
#include "SVGBrush.h"

namespace Visualization {
    namespace SVG {

        // Concrete factory for SVG visualization components
        // Cohesive - creates all SVG-related objects
        // Primitive - just creation, no complex logic
        class SVGFactory : public IVisualizationFactory {
        public:
            ICanvas* createCanvas(int width, int height) const {
                return new SVGCanvas(width, height);
            }

            IRenderer* createRenderer() const {
                return new SVGRenderer();
            }

            IBrush* createBrush(const std::string& strokeColor,
                const std::string& fillColor,
                int strokeWidth) const {
                return new SVGBrush(strokeColor, fillColor, strokeWidth);
            }
        };

    } // namespace SVG
} // namespace Visualization