#pragma once
#include <string>

namespace Painting {

    // Pen - defines line drawing style
    // Cohesive - encapsulates pen properties only
    // Primitive - simple data holder
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

} // namespace Painting