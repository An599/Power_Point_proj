#pragma once
#include <string>

namespace Painting {

    class Pen;
    class Brush;

    class IPainter {
    public:
        virtual ~IPainter() {}

        virtual void drawLine(int x1, int y1, int x2, int y2, const Pen& pen) = 0;
        virtual void drawEllipse(int centerX, int centerY, int radiusX, int radiusY,
            const Pen& pen, const Brush& brush) = 0;
        virtual void drawPolygon(const int* xPoints, const int* yPoints, int numPoints,
            const Pen& pen, const Brush& brush) = 0;
        virtual void drawText(int x, int y, const std::string& text,
            const std::string& fontFamily, int fontSize,
            const std::string& color) = 0;

        virtual void beginPaint() = 0;
        virtual void endPaint() = 0;
        virtual int getWidth() const = 0;
        virtual int getHeight() const = 0;
    };

}
