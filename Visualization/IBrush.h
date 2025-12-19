#pragma once
#include <string>

namespace Visualization {

    // IBrush - Primitive abstraction for drawing style
    // Decoupled from specific drawing implementation (SVG, GUI, etc.)
    // Cohesive - encapsulates all style properties
    class IBrush {
    public:
        virtual ~IBrush() {}

        // Sufficient interface for all drawing needs
        virtual std::string getStrokeColor() const = 0;
        virtual std::string getFillColor() const = 0;
        virtual int getStrokeWidth() const = 0;
        virtual float getOpacity() const = 0;

        // Completeness - additional style properties
        virtual std::string getStrokeStyle() const = 0;  // solid, dashed, dotted
        virtual std::string getFontFamily() const = 0;
        virtual int getFontSize() const = 0;
    };

} // namespace Visualization