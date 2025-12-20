#pragma once
#include "../Model/Presentation.h"
#include <memory>
#include <string>

namespace Serialization {
    class IDeserialize {
    public:
        virtual ~IDeserialize() = default;
        virtual std::unique_ptr<Model::Presentation> load(const std::string& filename) const = 0;
    };

} 

