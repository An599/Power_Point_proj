#pragma once
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include "IShape.h"

namespace Model {

    class Slide {
        std::string title_;
        std::vector<std::unique_ptr<IShape>> shapes_;

    public:
        explicit Slide(std::string title = "Untitled")
            : title_(std::move(title)) {
        }

        void addShape(std::unique_ptr<IShape> shape) {
            shapes_.push_back(std::move(shape));
        }

        std::string serialize() const {
            std::string result = "SLIDE " + title_;
            for (const auto& shape : shapes_) {
                result += "\n  " + shape->serialize();
            }
            return result;
        }

        const std::string& title() const { return title_; }
        void setTitle(const std::string& title) { title_ = title; }
        size_t shapeCount() const { return shapes_.size(); }

        const std::vector<std::unique_ptr<IShape>>& getShapes() const {
            return shapes_;
        }
    };

} // namespace Model
