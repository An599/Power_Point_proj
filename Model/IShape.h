#pragma once
#include <string>
#include "BoundingBox.h"

namespace Painting {
    class IPainter;
}

namespace Model {

    // IShape - interface with draw method
    // Decoupled - uses IPainter interface, not concrete painter
    class IShape {
    public:
        virtual ~IShape() {}

        // Core properties
        virtual std::string serialize() const = 0;
        virtual std::string getType() const = 0;
        virtual std::string getDescription() const = 0;
        virtual BoundingBox getBoundingBox() const = 0;
        virtual std::string getColor() const = 0;
        virtual std::string getFillColor() const = 0;
        virtual std::string getText() const = 0;

        // Draw method - each shape knows how to draw itself
        virtual void draw(Painting::IPainter& painter) const = 0;

        // Clone method for deep copying shapes
        virtual std::unique_ptr<IShape> clone() const = 0;
    };

} // namespace Model