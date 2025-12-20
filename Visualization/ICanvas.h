#pragma once
#include <string>

namespace Visualization {
    class IBrush;
    class ICanvas {
    public:
        virtual ~ICanvas() {}
        virtual void drawRectangle(int x, int y, int width, int height, const IBrush& brush) = 0;
        virtual void drawCircle(int centerX, int centerY, int radius, const IBrush& brush) = 0;
        virtual void drawEllipse(int centerX, int centerY, int radiusX, int radiusY, const IBrush& brush) = 0;
        virtual void drawLine(int x1, int y1, int x2, int y2, const IBrush& brush) = 0;
        virtual void drawPolygon(const int* xPoints, const int* yPoints, int numPoints, const IBrush& brush) = 0;
        virtual void drawText(int x, int y, const std::string& text, const IBrush& brush) = 0;
        virtual void beginDrawing() = 0;
        virtual void endDrawing() = 0;
        virtual int getWidth() const = 0;
        virtual int getHeight() const = 0;

        virtual std::string getOutput() const = 0;
    };

} 
