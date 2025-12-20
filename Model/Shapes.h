
#pragma once
#include "IShape.h"
#include "BoundingBox.h"
#include "../Painting/IPainter.h"
#include "../Painting/Pen.h"
#include "../Painting/Brush.h"
#include <sstream>

namespace Model {
    class Rectangle : public IShape {
        BoundingBox bounds_;
        std::string color_;
        std::string fillColor_;
        std::string text_;

    public:
        Rectangle(int x, int y, int width, int height, std::string color = "black", std::string fillColor = "none", std::string text = "")
            : bounds_(x, y, width, height), color_(color), fillColor_(fillColor), text_(text) {
        }

        Rectangle(const BoundingBox& bounds, std::string color = "black", std::string fillColor = "none", std::string text = "")
            : bounds_(bounds), color_(color), fillColor_(fillColor), text_(text) {
        }

        std::string serialize() const {
            std::string result = "RECT " + bounds_.serialize() + " " + color_;
            if (!text_.empty()) result += " \"" + text_ + "\"";
            return result;
        }

        std::string getType() const { return "Rectangle"; }

        std::string getDescription() const {
            std::ostringstream oss;
            oss << "Rectangle " << bounds_.describe() << " [" << color_ << "]";
            if (!text_.empty()) oss << " text: \"" << text_ << "\"";
            return oss.str();
        }

        BoundingBox getBoundingBox() const { return bounds_; }
        std::string getColor() const { return color_; }
        std::string getFillColor() const { return fillColor_; }
        std::string getText() const { return text_; }

        void draw(Painting::IPainter& painter) const {
            Painting::Pen pen(color_, 2, Painting::Pen::Type::SOLID);
            Painting::Brush::Style brushStyle = (fillColor_ == "none" || fillColor_.empty()) 
                ? Painting::Brush::Style::NONE 
                : Painting::Brush::Style::SOLID;
            Painting::Brush brush(fillColor_, brushStyle);

            int xPoints[4] = {
                bounds_.getX(),
                bounds_.getRight(),
                bounds_.getRight(),
                bounds_.getX()
            };
            int yPoints[4] = {
                bounds_.getY(),
                bounds_.getY(),
                bounds_.getBottom(),
                bounds_.getBottom()
            };

            painter.drawPolygon(xPoints, yPoints, 4, pen, brush);

            if (!text_.empty()) {
                int textX = bounds_.getCenterX();
                int textY = bounds_.getCenterY();
                painter.drawText(textX, textY, text_, "Arial", 14, color_);
            }
        }

        std::unique_ptr<IShape> clone() const override {
            return std::make_unique<Rectangle>(bounds_, color_, fillColor_, text_);
        }
    };

    class Circle : public IShape {
        BoundingBox bounds_;
        std::string color_;
        std::string fillColor_;
        std::string text_;

    public:
        Circle(int x, int y, int width, int height, std::string color = "black", std::string fillColor = "none", std::string text = "")
            : bounds_(x, y, width, height), color_(color), fillColor_(fillColor), text_(text) {
        }

        Circle(const BoundingBox& bounds, std::string color = "black", std::string fillColor = "none", std::string text = "")
            : bounds_(bounds), color_(color), fillColor_(fillColor), text_(text) {
        }

        std::string serialize() const {
            std::string result = "CIRCLE " + bounds_.serialize() + " " + color_;
            if (!text_.empty()) result += " \"" + text_ + "\"";
            return result;
        }

        std::string getType() const { return "Circle"; }

        std::string getDescription() const {
            std::ostringstream oss;
            oss << "Circle " << bounds_.describe() << " [" << color_ << "]";
            if (!text_.empty()) oss << " text: \"" << text_ << "\"";
            return oss.str();
        }

        BoundingBox getBoundingBox() const { return bounds_; }
        std::string getColor() const { return color_; }
        std::string getFillColor() const { return fillColor_; }
        std::string getText() const { return text_; }

        void draw(Painting::IPainter& painter) const {
            Painting::Pen pen(color_, 2, Painting::Pen::Type::SOLID);
            Painting::Brush::Style brushStyle = (fillColor_ == "none" || fillColor_.empty()) 
                ? Painting::Brush::Style::NONE 
                : Painting::Brush::Style::SOLID;
            Painting::Brush brush(fillColor_, brushStyle);

            int centerX = bounds_.getCenterX();
            int centerY = bounds_.getCenterY();
            int radiusX = bounds_.getWidth() / 2;
            int radiusY = bounds_.getHeight() / 2;

            painter.drawEllipse(centerX, centerY, radiusX, radiusY, pen, brush);

            if (!text_.empty()) {
                painter.drawText(centerX, centerY, text_, "Arial", 14, color_);
            }
        }

        std::unique_ptr<IShape> clone() const override {
            return std::make_unique<Circle>(bounds_, color_, fillColor_, text_);
        }
    };
    class Triangle : public IShape {
        BoundingBox bounds_;
        std::string color_;
        std::string fillColor_;
        std::string text_;

    public:
        Triangle(int x, int y, int width, int height, std::string color = "black", std::string fillColor = "none", std::string text = "")
            : bounds_(x, y, width, height), color_(color), fillColor_(fillColor), text_(text) {
        }

        Triangle(const BoundingBox& bounds, std::string color = "black", std::string fillColor = "none", std::string text = "")
            : bounds_(bounds), color_(color), fillColor_(fillColor), text_(text) {
        }

        std::string serialize() const {
            std::string result = "TRIANGLE " + bounds_.serialize() + " " + color_;
            if (!text_.empty()) result += " \"" + text_ + "\"";
            return result;
        }

        std::string getType() const { return "Triangle"; }

        std::string getDescription() const {
            std::ostringstream oss;
            oss << "Triangle " + bounds_.describe() << " [" << color_ << "]";
            if (!text_.empty()) oss << " text: \"" << text_ << "\"";
            return oss.str();
        }

        BoundingBox getBoundingBox() const { return bounds_; }
        std::string getColor() const { return color_; }
        std::string getFillColor() const { return fillColor_; }
        std::string getText() const { return text_; }

        void draw(Painting::IPainter& painter) const {
            Painting::Pen pen(color_, 2, Painting::Pen::Type::SOLID);
            Painting::Brush::Style brushStyle = (fillColor_ == "none" || fillColor_.empty()) 
                ? Painting::Brush::Style::NONE 
                : Painting::Brush::Style::SOLID;
            Painting::Brush brush(fillColor_, brushStyle);
            int xPoints[3] = {
                bounds_.getX(),              // Bottom left
                bounds_.getRight(),          // Bottom right
                bounds_.getCenterX()         // Top center
            };
            int yPoints[3] = {
                bounds_.getBottom(),         // Bottom left
                bounds_.getBottom(),         // Bottom right
                bounds_.getY()               // Top center
            };

            painter.drawPolygon(xPoints, yPoints, 3, pen, brush);

            if (!text_.empty()) {
                int textX = bounds_.getCenterX();
                int textY = bounds_.getCenterY();
                painter.drawText(textX, textY, text_, "Arial", 14, color_);
            }
        }

        std::unique_ptr<IShape> clone() const override {
            return std::make_unique<Triangle>(bounds_, color_, fillColor_, text_);
        }
    };

    class Trapezoid : public IShape {
        BoundingBox bounds_;
        std::string color_;
        std::string fillColor_;
        std::string text_;

    public:
        Trapezoid(int x, int y, int width, int height, std::string color = "black", std::string fillColor = "none", std::string text = "")
            : bounds_(x, y, width, height), color_(color), fillColor_(fillColor), text_(text) {
        }

        Trapezoid(const BoundingBox& bounds, std::string color = "black", std::string fillColor = "none", std::string text = "")
            : bounds_(bounds), color_(color), fillColor_(fillColor), text_(text) {
        }

        std::string serialize() const {
            std::string result = "TRAPEZOID " + bounds_.serialize() + " " + color_;
            if (!text_.empty()) result += " \"" + text_ + "\"";
            return result;
        }

        std::string getType() const { return "Trapezoid"; }

        std::string getDescription() const {
            std::ostringstream oss;
            oss << "Trapezoid " << bounds_.describe() << " [" << color_ << "]";
            if (!text_.empty()) oss << " text: \"" << text_ << "\"";
            return oss.str();
        }

        BoundingBox getBoundingBox() const { return bounds_; }
        std::string getColor() const { return color_; }
        std::string getFillColor() const { return fillColor_; }
        std::string getText() const { return text_; }

        void draw(Painting::IPainter& painter) const {
            Painting::Pen pen(color_, 2, Painting::Pen::Type::SOLID);
            Painting::Brush::Style brushStyle = (fillColor_ == "none" || fillColor_.empty()) 
                ? Painting::Brush::Style::NONE 
                : Painting::Brush::Style::SOLID;
            Painting::Brush brush(fillColor_, brushStyle);

            int indent = bounds_.getWidth() / 4;
            int xPoints[4] = {
                bounds_.getX(),              // Bottom left
                bounds_.getRight(),          // Bottom right
                bounds_.getRight() - indent, // Top right
                bounds_.getX() + indent      // Top left
            };
            int yPoints[4] = {
                bounds_.getBottom(),         // Bottom left
                bounds_.getBottom(),         // Bottom right
                bounds_.getY(),              // Top right
                bounds_.getY()               // Top left
            };

            painter.drawPolygon(xPoints, yPoints, 4, pen, brush);
            if (!text_.empty()) {
                painter.drawText(bounds_.getCenterX(), bounds_.getCenterY(), text_, "Arial", 14, color_);
            }
        }

        std::unique_ptr<IShape> clone() const override {
            return std::make_unique<Trapezoid>(bounds_, color_, fillColor_, text_);
        }
    };

    class Parallelogram : public IShape {
        BoundingBox bounds_;
        std::string color_;
        std::string fillColor_;
        std::string text_;

    public:
        Parallelogram(int x, int y, int width, int height, std::string color = "black", std::string fillColor = "none", std::string text = "")
            : bounds_(x, y, width, height), color_(color), fillColor_(fillColor), text_(text) {
        }

        Parallelogram(const BoundingBox& bounds, std::string color = "black", std::string fillColor = "none", std::string text = "")
            : bounds_(bounds), color_(color), fillColor_(fillColor), text_(text) {
        }

        std::string serialize() const {
            std::string result = "PARALLELOGRAM " + bounds_.serialize() + " " + color_;
            if (!text_.empty()) result += " \"" + text_ + "\"";
            return result;
        }

        std::string getType() const { return "Parallelogram"; }

        std::string getDescription() const {
            std::ostringstream oss;
            oss << "Parallelogram " << bounds_.describe() << " [" << color_ << "]";
            if (!text_.empty()) oss << " text: \"" << text_ << "\"";
            return oss.str();
        }

        BoundingBox getBoundingBox() const { return bounds_; }
        std::string getColor() const { return color_; }
        std::string getFillColor() const { return fillColor_; }
        std::string getText() const { return text_; }

        void draw(Painting::IPainter& painter) const {
            Painting::Pen pen(color_, 2, Painting::Pen::Type::SOLID);
            Painting::Brush::Style brushStyle = (fillColor_ == "none" || fillColor_.empty()) 
                ? Painting::Brush::Style::NONE 
                : Painting::Brush::Style::SOLID;
            Painting::Brush brush(fillColor_, brushStyle);

            int slant = bounds_.getWidth() / 5;
            int xPoints[4] = {
                bounds_.getX(),              // Bottom left
                bounds_.getRight() - slant,  // Bottom right
                bounds_.getRight(),          // Top right
                bounds_.getX() + slant       // Top left
            };
            int yPoints[4] = {
                bounds_.getBottom(),         // Bottom left
                bounds_.getBottom(),         // Bottom right
                bounds_.getY(),              // Top right
                bounds_.getY()               // Top left
            };

            painter.drawPolygon(xPoints, yPoints, 4, pen, brush);
            if (!text_.empty()) {
                painter.drawText(bounds_.getCenterX(), bounds_.getCenterY(), text_, "Arial", 14, color_);
            }
        }

        std::unique_ptr<IShape> clone() const override {
            return std::make_unique<Parallelogram>(bounds_, color_, fillColor_, text_);
        }
    };

    class Rhombus : public IShape {
        BoundingBox bounds_;
        std::string color_;
        std::string fillColor_;
        std::string text_;

    public:
        Rhombus(int x, int y, int width, int height, std::string color = "black", std::string fillColor = "none", std::string text = "")
            : bounds_(x, y, width, height), color_(color), fillColor_(fillColor), text_(text) {
        }

        Rhombus(const BoundingBox& bounds, std::string color = "black", std::string fillColor = "none", std::string text = "")
            : bounds_(bounds), color_(color), fillColor_(fillColor), text_(text) {
        }

        std::string serialize() const {
            std::string result = "RHOMBUS " + bounds_.serialize() + " " + color_;
            if (!text_.empty()) result += " \"" + text_ + "\"";
            return result;
        }

        std::string getType() const { return "Rhombus"; }

        std::string getDescription() const {
            std::ostringstream oss;
            oss << "Rhombus " << bounds_.describe() << " [" << color_ << "]";
            if (!text_.empty()) oss << " text: \"" << text_ << "\"";
            return oss.str();
        }

        BoundingBox getBoundingBox() const { return bounds_; }
        std::string getColor() const { return color_; }
        std::string getFillColor() const { return fillColor_; }
        std::string getText() const { return text_; }
        void draw(Painting::IPainter& painter) const {
            Painting::Pen pen(color_, 2, Painting::Pen::Type::SOLID);
            Painting::Brush::Style brushStyle = (fillColor_ == "none" || fillColor_.empty()) 
                ? Painting::Brush::Style::NONE 
                : Painting::Brush::Style::SOLID;
            Painting::Brush brush(fillColor_, brushStyle);

            int xPoints[4] = {
                bounds_.getX(),              // Left
                bounds_.getCenterX(),        // Top
                bounds_.getRight(),          // Right
                bounds_.getCenterX()         // Bottom
            };
            int yPoints[4] = {
                bounds_.getCenterY(),        // Left
                bounds_.getY(),              // Top
                bounds_.getCenterY(),        // Right
                bounds_.getBottom()          // Bottom
            };

            painter.drawPolygon(xPoints, yPoints, 4, pen, brush);
            if (!text_.empty()) {
                painter.drawText(bounds_.getCenterX(), bounds_.getCenterY(), text_, "Arial", 14, color_);
            }
        }

        std::unique_ptr<IShape> clone() const override {
            return std::make_unique<Rhombus>(bounds_, color_, fillColor_, text_);
        }
    };

} 
