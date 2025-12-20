#pragma once
#include <string>

namespace Painting {

    class Pen {
    public:
        enum class Type {
            SOLID,
            DASHED,
            DOTTED
        };

    private:
        std::string color_;
        int width_;
        Type type_;

    public:
        Pen(std::string color = "black", int width = 1, Type type = Type::SOLID)
            : color_(color), width_(width), type_(type) {
        }

        const std::string& getColor() const { return color_; }
        int getWidth() const { return width_; }
        Type getType() const { return type_; }

        void setColor(const std::string& color) { color_ = color; }
        void setWidth(int width) { width_ = width; }
        void setType(Type type) { type_ = type; }
    };

}

