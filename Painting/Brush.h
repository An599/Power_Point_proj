#pragma once
#include <string>

namespace Painting {
    class Brush {
    public:
        enum class Style {
            SOLID,
            NONE,
            GRADIENT
        };

    private:
        std::string color_;
        Style style_;
        float opacity_;

    public:
        Brush(std::string color = "none", Style style = Style::NONE, float opacity = 1.0f)
            : color_(color), style_(style), opacity_(opacity) {
        }

        const std::string& getColor() const { return color_; }
        Style getStyle() const { return style_; }
        float getOpacity() const { return opacity_; }

        void setColor(const std::string& color) { color_ = color; }
        void setStyle(Style style) { style_ = style; }
        void setOpacity(float opacity) { opacity_ = opacity; }
    };

} 
