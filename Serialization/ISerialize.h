#pragma once
#include "../Model/Presentation.h"
#include <string>

namespace Serialization {

    class ISerialize {
    public:
        virtual ~ISerialize() = default;

        virtual void save(const Model::Presentation& presentation, const std::string& filename) const = 0;
    };

}
