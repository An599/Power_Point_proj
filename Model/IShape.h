#pragma once
#include <string>

namespace Model {

    class IShape {
    public:
        virtual ~IShape() = default;
        virtual std::string serialize() const = 0;
        virtual std::string getType() const = 0;
        virtual std::string getDescription() const = 0;
    };

}