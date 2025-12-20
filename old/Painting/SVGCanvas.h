#pragma once
#include "Painting/SVGPainter.h"
#include <memory>
#include <string>

namespace Painting {

    // SVGCanvas - concrete canvas (no interface)
    // Cohesive - owns SVGPainter and manages canvas lifecycle
    class SVGCanvas {
    private:
        std::unique_ptr<SVGPainter> painter_;

    public:
        explicit SVGCanvas(int width = 800, int height = 600)
            : painter_(new SVGPainter(width, height)) {
        }

        // Access to painter
        SVGPainter& getPainter() {
            return *painter_;
        }

        const SVGPainter& getPainter() const {
            return *painter_;
        }

        // Convenience methods
        void beginDrawing() {
            painter_->beginPaint();
        }

        void endDrawing() {
            painter_->endPaint();
        }

        std::string getOutput() const {
            return painter_->getSVG();
        }

        int getWidth() const {
            return painter_->getWidth();
        }

        int getHeight() const {
            return painter_->getHeight();
        }
    };

} // namespace Painting