#pragma once
#include <string>

namespace Painting {

    // Forward declarations
    class Pen;
    class Brush;

    // IPainter - interface for drawing primitives
    // Decoupled from specific implementation (SVG, Qt, OpenGL, etc.)
    // Sufficient - provides all primitive drawing operations
    class IPainter {
    public:
        virtual ~IPainter() {}

        // Primitive drawing operations
        virtual void drawLine(int x1, int y1, int x2, int y2, const Pen& pen) = 0;
        virtual void drawEllipse(int centerX, int centerY, int radiusX, int radiusY,
            const Pen& pen, const Brush& brush) = 0;
        virtual void drawPolygon(const int* xPoints, const int* yPoints, int numPoints,
            const Pen& pen, const Brush& brush) = 0;
        virtual void drawText(int x, int y, const std::string& text,
            const std::string& fontFamily, int fontSize,
            const std::string& color) = 0;

        // Canvas management
        virtual void beginPaint() = 0;
        virtual void endPaint() = 0;
        virtual int getWidth() const = 0;
        virtual int getHeight() const = 0;
    };

} // namespace Painting