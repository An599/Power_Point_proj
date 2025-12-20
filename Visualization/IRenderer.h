#pragma once

namespace Visualization {

    class ICanvas;
    class IBrush;

} 

namespace Model {
    class IShape;
    class Slide;
    class Presentation;
} 

namespace Visualization {

    class IRenderer {
    public:
        virtual ~IRenderer() {}
        virtual void renderShape(const Model::IShape& shape, ICanvas& canvas, const IBrush& brush) = 0;
        virtual void renderSlide(const Model::Slide& slide, ICanvas& canvas) = 0;
        virtual void renderPresentation(const Model::Presentation& presentation, ICanvas& canvas) = 0;
        virtual IBrush* createBrush(const std::string& strokeColor,
            const std::string& fillColor,
            int strokeWidth = 1) const = 0;
    };

} 
