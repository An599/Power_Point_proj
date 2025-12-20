#pragma once
#include <string>
#include "BoundingBox.h"

namespace Painting {
    class IPainter;
}

namespace Model {
    class IShape {
    public:
        virtual ~IShape() {}
        virtual std::string serialize() const = 0;
        virtual std::string getType() const = 0;
        virtual std::string getDescription() const = 0;
        virtual BoundingBox getBoundingBox() const = 0;
        virtual std::string getColor() const = 0;
        virtual std::string getFillColor() const = 0;
        virtual std::string getText() const = 0;
        virtual void draw(Painting::IPainter& painter) const = 0;
        virtual std::unique_ptr<IShape> clone() const = 0;
    };

} 
