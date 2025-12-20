#pragma once
#include <string>
#include <sstream>

namespace Model {
    class BoundingBox {
        int x_, y_, width_, height_;

    public:
        BoundingBox(int x, int y, int width, int height)
            : x_(x), y_(y), width_(width), height_(height) {
        }

        int getX() const { return x_; }
        int getY() const { return y_; }
        int getWidth() const { return width_; }
        int getHeight() const { return height_; }
        int getRight() const { return x_ + width_; }
        int getBottom() const { return y_ + height_; }
        int getCenterX() const { return x_ + width_ / 2; }
        int getCenterY() const { return y_ + height_ / 2; }

        std::string serialize() const {
            std::ostringstream oss;
            oss << x_ << " " << y_ << " " << width_ << " " << height_;
            return oss.str();
        }

        std::string describe() const {
            std::ostringstream oss;
            oss << "at (" << x_ << "," << y_ << ") " << width_ << "x" << height_;
            return oss.str();
        }
    };

}
