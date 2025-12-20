#pragma once
#include "Painting/SVGPainter.h"
#include <memory>
#include <string>

namespace Painting {
    class SVGCanvas {
    private:
        std::unique_ptr<SVGPainter> painter_;

    public:
        explicit SVGCanvas(int width = 800, int height = 600)
            : painter_(new SVGPainter(width, height)) {
        }
        SVGPainter& getPainter() {
            return *painter_;
        }

        const SVGPainter& getPainter() const {
            return *painter_;
        }
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

} 
