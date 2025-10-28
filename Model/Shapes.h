#pragma once
#include "IShape.h"
#include <sstream>

namespace Model {

    class Rectangle : public IShape {
        int x_, y_, width_, height_;
        std::string color_;
        std::string text_;

    public:
        Rectangle(int x, int y, int width, int height, std::string color = "black", std::string text = "")
            : x_(x), y_(y), width_(width), height_(height), color_(std::move(color)), text_(std::move(text)) {
        }

        std::string serialize() const override {
            std::string result = "RECT " + std::to_string(x_) + " " + std::to_string(y_) + " " +
                std::to_string(width_) + " " + std::to_string(height_) + " " + color_;
            if (!text_.empty()) {
                result += " \"" + text_ + "\"";
            }
            return result;
        }

        std::string getType() const override {
            return "Rectangle";
        }

        std::string getDescription() const override {
            std::ostringstream oss;
            oss << "Rectangle at (" << x_ << "," << y_ << ") "
                << width_ << "x" << height_ << " [" << color_ << "]";
            if (!text_.empty()) {
                oss << " text: \"" << text_ << "\"";
            }
            return oss.str();
        }
    };

    class Circle : public IShape {
        int x_, y_, radius_;
        std::string color_;
        std::string text_;

    public:
        Circle(int x, int y, int radius, std::string color = "black", std::string text = "")
            : x_(x), y_(y), radius_(radius), color_(std::move(color)), text_(std::move(text)) {
        }

        std::string serialize() const override {
            std::string result = "CIRCLE " + std::to_string(x_) + " " + std::to_string(y_) + " " +
                std::to_string(radius_) + " " + color_;
            if (!text_.empty()) {
                result += " \"" + text_ + "\"";
            }
            return result;
        }

        std::string getType() const override {
            return "Circle";
        }

        std::string getDescription() const override {
            std::ostringstream oss;
            oss << "Circle at (" << x_ << "," << y_ << ") radius=" << radius_ << " [" << color_ << "]";
            if (!text_.empty()) {
                oss << " text: \"" << text_ << "\"";
            }
            return oss.str();
        }
    };

    class Triangle : public IShape {
        int x1_, y1_, x2_, y2_, x3_, y3_;
        std::string color_;
        std::string text_;

    public:
        Triangle(int x1, int y1, int x2, int y2, int x3, int y3, std::string color = "black", std::string text = "")
            : x1_(x1), y1_(y1), x2_(x2), y2_(y2), x3_(x3), y3_(y3), color_(std::move(color)), text_(std::move(text)) {
        }

        std::string serialize() const override {
            std::string result = "TRIANGLE " + std::to_string(x1_) + " " + std::to_string(y1_) + " " +
                std::to_string(x2_) + " " + std::to_string(y2_) + " " +
                std::to_string(x3_) + " " + std::to_string(y3_) + " " + color_;
            if (!text_.empty()) {
                result += " \"" + text_ + "\"";
            }
            return result;
        }

        std::string getType() const override {
            return "Triangle";
        }

        std::string getDescription() const override {
            std::ostringstream oss;
            oss << "Triangle (" << x1_ << "," << y1_ << ") (" << x2_ << "," << y2_ << ") ("
                << x3_ << "," << y3_ << ") [" << color_ << "]";
            if (!text_.empty()) {
                oss << " text: \"" << text_ << "\"";
            }
            return oss.str();
        }
    };

} // namespace Model