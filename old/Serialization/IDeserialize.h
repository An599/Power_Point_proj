#pragma once
#include "../Model/Presentation.h"
#include <memory>
#include <string>

namespace Serialization {

    // IDeserialize - interface for deserializing presentations from files
    class IDeserialize {
    public:
        virtual ~IDeserialize() = default;

        // Load presentation from a file
        virtual std::unique_ptr<Model::Presentation> load(const std::string& filename) const = 0;
    };

} // namespace Serialization

