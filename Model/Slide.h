#pragma once
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include "IShape.h"

namespace Model {

    class Slide {
        std::vector<std::unique_ptr<IShape> > shapes_;

    public:
        explicit Slide() {
        }

        void addShape(std::unique_ptr<IShape> shape) {
            shapes_.push_back(std::move(shape));
        }

        void removeLastShape() {
            if (!shapes_.empty()) {
                shapes_.pop_back();
            }
        }

        std::unique_ptr<IShape> removeShapeAt(size_t index) {
            if (index >= shapes_.size()) {
                return nullptr;
            }
            auto shape = std::move(shapes_[index]);
            shapes_.erase(shapes_.begin() + index);
            return shape;
        }

        std::string serialize() const {
            std::string result = "SLIDE";
            for (size_t i = 0; i < shapes_.size(); ++i) {
                result += "\n  " + shapes_[i]->serialize();
            }
            return result;
        }
        size_t shapeCount() const { return shapes_.size(); }

        const std::vector<std::unique_ptr<IShape> >& getShapes() const {
            return shapes_;
        }

        std::vector<IShape*> getShapesByZOrder() const {
            std::vector<IShape*> result;
            result.reserve(shapes_.size());
            for (const auto& shape : shapes_) {
                if (shape) {
                    result.push_back(shape.get());
                }
            }
            return result;
        }

        void bringToFront(size_t index) {
            if (index < shapes_.size()) {
                auto shape = std::move(shapes_[index]);
                shapes_.erase(shapes_.begin() + index);
                shapes_.push_back(std::move(shape));
            }
        }

        std::unique_ptr<Slide> clone() const {
            auto cloned = std::make_unique<Slide>();
            for (const auto& shape : shapes_) {
                if (shape) {
                    cloned->addShape(shape->clone());
                }
            }
            return cloned;
        }
    };

}
