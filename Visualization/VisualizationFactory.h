#pragma once
#include "ICanvas.h"
#include "IRenderer.h"
#include "IBrush.h"
#include <memory>
#include <string>

namespace Visualization {
    class IVisualizationFactory {
    public:
        virtual ~IVisualizationFactory() {}

        virtual ICanvas* createCanvas(int width, int height) const = 0;
        virtual IRenderer* createRenderer() const = 0;
        virtual IBrush* createBrush(const std::string& strokeColor,
            const std::string& fillColor,
            int strokeWidth = 1) const = 0;
    };

}
