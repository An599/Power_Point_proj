#pragma once
#include "../Model/Presentation.h"
#include <string>

namespace Visualization {

    // IVisualization - interface for visualizing presentations
    // Provides a high-level interface to generate visual representations
    class IVisualization {
    public:
        virtual ~IVisualization() = default;

        // Generate visual representation of the presentation
        // Returns a string representation (e.g., SVG, HTML, etc.)
        virtual std::string visualize(const Model::Presentation& presentation) const = 0;
    };

} // namespace Visualization

