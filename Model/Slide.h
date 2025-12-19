#pragma once
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include "IShape.h"

namespace Model {

    // Slide - cohesive container for shapes
    // Sufficient - provides all needed shape management operations
    class Slide {
        std::vector<std::unique_ptr<IShape> > shapes_;

    public:
        explicit Slide() {
        }

        void addShape(std::unique_ptr<IShape> shape) {
            shapes_.push_back(std::move(shape));
        }

        // Completeness - supports undo operation
        void removeLastShape() {
            if (!shapes_.empty()) {
                shapes_.pop_back();
            }
        }

        // Remove shape at specific index (for undo/redo)
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

        // Get shapes sorted by Z-order (back to front)
        // In our implementation, the vector order represents Z-order
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

        // Bring shape at index to front (move to end of vector)
        void bringToFront(size_t index) {
            if (index < shapes_.size()) {
                auto shape = std::move(shapes_[index]);
                shapes_.erase(shapes_.begin() + index);
                shapes_.push_back(std::move(shape));
            }
        }

        // Clone method - creates a deep copy of the slide with all its shapes
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

} // namespace Model
