#pragma once

namespace Visualization {

    // Forward declarations (decoupling)
    class ICanvas;
    class IBrush;

} // namespace Visualization

namespace Model {
    // Forward declarations (decoupling from Model layer)
    class IShape;
    class Slide;
    class Presentation;
} // namespace Model

namespace Visualization {

    // IRenderer - Abstraction for "someone who draws"
    // Decoupled from both Model and Visualization implementation
    // Cohesive - responsible only for translating Model to Canvas
    // Follows Visitor pattern for extensibility
    class IRenderer {
    public:
        virtual ~IRenderer() {}

        // Primitive operations - render individual elements
        virtual void renderShape(const Model::IShape& shape, ICanvas& canvas, const IBrush& brush) = 0;
        virtual void renderSlide(const Model::Slide& slide, ICanvas& canvas) = 0;
        virtual void renderPresentation(const Model::Presentation& presentation, ICanvas& canvas) = 0;

        // Factory method for brush creation (decoupling)
        virtual IBrush* createBrush(const std::string& strokeColor,
            const std::string& fillColor,
            int strokeWidth = 1) const = 0;
    };

} // namespace Visualization
