#pragma once
#include "../Model/Presentation.h"
#include <string>

namespace Serialization {

    // ISerialize - interface for serializing presentations to files
    class ISerialize {
    public:
        virtual ~ISerialize() = default;

        // Save presentation to a file
        virtual void save(const Model::Presentation& presentation, const std::string& filename) const = 0;
    };

} // namespace Serialization

