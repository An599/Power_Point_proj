#pragma once
#include "../Model/Presentation.h"
#include <string>

namespace Visualization {
    class IVisualization {
    public:
        virtual ~IVisualization() = default;
        virtual std::string visualize(const Model::Presentation& presentation) const = 0;
    };

} 

