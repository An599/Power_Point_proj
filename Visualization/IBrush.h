#pragma once
#include <string>

namespace Visualization {

    class IBrush {
    public:
        virtual ~IBrush() {}
        virtual std::string getStrokeColor() const = 0;
        virtual std::string getFillColor() const = 0;
        virtual int getStrokeWidth() const = 0;
        virtual float getOpacity() const = 0;

        virtual std::string getStrokeStyle() const = 0; 
        virtual std::string getFontFamily() const = 0;
        virtual int getFontSize() const = 0;
    };

} 
