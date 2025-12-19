#pragma once
#include "IPainter.h"
#include "Pen.h"
#include "Brush.h"
#include <vector>

namespace Painting {

    // TransformedPainter - wrapper that applies coordinate transformation
    // Used to offset shapes into slide areas
    class TransformedPainter : public IPainter {
    private:
        IPainter& basePainter_;
        int offsetX_;
        int offsetY_;

    public:
        TransformedPainter(IPainter& basePainter, int offsetX, int offsetY)
            : basePainter_(basePainter), offsetX_(offsetX), offsetY_(offsetY) {
        }

        void drawLine(int x1, int y1, int x2, int y2, const Pen& pen) override {
            basePainter_.drawLine(x1 + offsetX_, y1 + offsetY_, x2 + offsetX_, y2 + offsetY_, pen);
        }

        void drawEllipse(int centerX, int centerY, int radiusX, int radiusY,
            const Pen& pen, const Brush& brush) override {
            basePainter_.drawEllipse(centerX + offsetX_, centerY + offsetY_, radiusX, radiusY, pen, brush);
        }

        void drawPolygon(const int* xPoints, const int* yPoints, int numPoints,
            const Pen& pen, const Brush& brush) override {
            // Create transformed arrays
            std::vector<int> transformedX(numPoints);
            std::vector<int> transformedY(numPoints);
            for (int i = 0; i < numPoints; ++i) {
                transformedX[i] = xPoints[i] + offsetX_;
                transformedY[i] = yPoints[i] + offsetY_;
            }
            basePainter_.drawPolygon(transformedX.data(), transformedY.data(), numPoints, pen, brush);
        }

        void drawText(int x, int y, const std::string& text,
            const std::string& fontFamily, int fontSize,
            const std::string& color) override {
            basePainter_.drawText(x + offsetX_, y + offsetY_, text, fontFamily, fontSize, color);
        }

        void beginPaint() override {
            basePainter_.beginPaint();
        }

        void endPaint() override {
            basePainter_.endPaint();
        }

        int getWidth() const override {
            return basePainter_.getWidth();
        }

        int getHeight() const override {
            return basePainter_.getHeight();
        }
    };

} // namespace Painting

