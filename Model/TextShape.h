#pragma once
#include "IShape.h"
#include "BoundingBox.h"
#include "../Painting/IPainter.h"
#include "../Painting/Pen.h"
#include "../Painting/Brush.h"
#include <sstream>

namespace Model {

    class TextShape : public IShape {
        BoundingBox bounds_;
        std::string text_;
        std::string textColor_;

    public:
        TextShape(int x, int y, int width, int height, std::string text, std::string textColor = "black")
            : bounds_(x, y, width, height), text_(text), textColor_(textColor) {
        }

        TextShape(const BoundingBox& bounds, std::string text, std::string textColor = "black")
            : bounds_(bounds), text_(text), textColor_(textColor) {
        }

        std::string serialize() const override {
            return "TEXT " + bounds_.serialize() + " \"" + text_ + "\"";
        }

        std::string getType() const override { return "Text"; }

        std::string getDescription() const override {
            std::ostringstream oss;
            oss << "Text " << bounds_.describe() << " \"" << text_ << "\"";
            return oss.str();
        }

        BoundingBox getBoundingBox() const override { return bounds_; }
        std::string getColor() const override { return "none"; }
        std::string getFillColor() const override { return "none"; }
        std::string getText() const override { return text_; }

        void draw(Painting::IPainter& painter) const override {

            if (!text_.empty()) {
                int textX = bounds_.getCenterX();
                int textY = bounds_.getCenterY();
                painter.drawText(textX, textY, text_, "Arial", 14, textColor_);
            }
        }

        std::unique_ptr<IShape> clone() const override {
            return std::make_unique<TextShape>(bounds_, text_, textColor_);
        }
    };

}

