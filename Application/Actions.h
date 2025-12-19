#pragma once
#include "IAction.h"
#include "../Model/Slide.h"
#include "../Model/IShape.h"
#include <memory>
#include <string>
#include <vector>

namespace Application {

    // Forward declarations
    class RemoveSlideAction;
    class RemoveShapeAction;
    class AddShapeAction;

    // RemoveSlideAction - removes a slide from the presentation
    // Returns AddSlideAction as converse action
    class RemoveSlideAction : public IAction {
        size_t position_;
        std::unique_ptr<Model::Slide> removedSlide_;

    public:
        explicit RemoveSlideAction(size_t position) : position_(position) {
        }

        std::unique_ptr<IAction> execute(Model::Presentation* presentation) override;
    };

    // AddSlideAction - adds a slide to the presentation
    // Returns RemoveSlideAction as converse action
    class AddSlideAction : public IAction {
        std::unique_ptr<Model::Slide> slide_;
        size_t position_;
        bool hasPosition_;

    public:
        AddSlideAction(std::unique_ptr<Model::Slide> slide, size_t position = static_cast<size_t>(-1), bool hasPosition = false)
            : slide_(std::move(slide)), position_(position), hasPosition_(hasPosition) {
        }

        std::unique_ptr<IAction> execute(Model::Presentation* presentation) override {
            if (!presentation) {
                return nullptr;
            }

            size_t actualPosition = 0;
            if (hasPosition_) {
                size_t slideCount = presentation->slideCount();
                if (position_ > slideCount) {
                    // Invalid position - addSlide will show warning and add at end
                    actualPosition = slideCount;
                    presentation->addSlide(std::move(slide_), position_);
                }
                else {
                    actualPosition = position_;
                    presentation->addSlide(std::move(slide_), position_);
                }
            }
            else {
                actualPosition = presentation->slideCount();
                presentation->addSlide(std::move(slide_));
            }

            // Return RemoveSlideAction as converse action
            // After adding, the slide is at actualPosition
            return std::make_unique<RemoveSlideAction>(actualPosition);
        }
    };

    // Implementation of RemoveSlideAction::execute (after AddSlideAction is defined)
    inline std::unique_ptr<IAction> RemoveSlideAction::execute(Model::Presentation* presentation) {
        if (!presentation || position_ >= presentation->slideCount()) {
            return nullptr;
        }

        // Clone the slide before removing it
        Model::Slide* slidePtr = presentation->getSlide(position_);
        removedSlide_ = slidePtr->clone();

        presentation->removeSlide(position_);

        // Return AddSlideAction as converse action
        return std::make_unique<AddSlideAction>(std::move(removedSlide_), position_, true);
    }

    // RemoveShapeAction - removes a shape from a slide
    // Returns AddShapeAction as converse action
    class RemoveShapeAction : public IAction {
        size_t slideIndex_;
        size_t shapeIndex_;
        std::unique_ptr<Model::IShape> removedShape_;

    public:
        RemoveShapeAction(size_t slideIndex, size_t shapeIndex)
            : slideIndex_(slideIndex), shapeIndex_(shapeIndex) {
        }

        std::unique_ptr<IAction> execute(Model::Presentation* presentation) override;
    };

    // AddShapeAction - adds a shape to a slide
    // Returns RemoveShapeAction as converse action
    class AddShapeAction : public IAction {
        size_t slideIndex_;
        std::unique_ptr<Model::IShape> shape_;
        bool toFront_;
        size_t actualShapeIndex_; // Store where shape was actually added

    public:
        AddShapeAction(size_t slideIndex, std::unique_ptr<Model::IShape> shape, bool toFront = false)
            : slideIndex_(slideIndex), shape_(std::move(shape)), toFront_(toFront), actualShapeIndex_(0) {
        }

        std::unique_ptr<IAction> execute(Model::Presentation* presentation) override {
            if (!presentation || slideIndex_ >= presentation->slideCount()) {
                return nullptr;
            }

            Model::Slide* slide = presentation->getSlide(slideIndex_);
            size_t originalCount = slide->shapeCount();
            
            // Add the shape
            slide->addShape(std::move(shape_));
            
            // Store the index where shape was added (before bringing to front)
            actualShapeIndex_ = originalCount;
            
            // Bring to front if requested
            if (toFront_ && actualShapeIndex_ < slide->shapeCount()) {
                slide->bringToFront(actualShapeIndex_);
                actualShapeIndex_ = slide->shapeCount() - 1; // After bringToFront, it's at the end
            }

            // Return RemoveShapeAction as converse action
            return std::make_unique<RemoveShapeAction>(slideIndex_, actualShapeIndex_);
        }
    };

    // Implementation of RemoveShapeAction::execute (after AddShapeAction is defined)
    inline std::unique_ptr<IAction> RemoveShapeAction::execute(Model::Presentation* presentation) {
        if (!presentation || slideIndex_ >= presentation->slideCount()) {
            return nullptr;
        }

        Model::Slide* slide = presentation->getSlide(slideIndex_);
        if (shapeIndex_ >= slide->shapeCount()) {
            return nullptr;
        }

        // Remove and clone the shape before removing
        Model::IShape* shapePtr = slide->getShapes()[shapeIndex_].get();
        removedShape_ = shapePtr->clone();
        
        // Remove the shape
        slide->removeShapeAt(shapeIndex_);

        // Return AddShapeAction as converse action
        // Note: We don't preserve toFront flag, shape will be added at the end
        return std::make_unique<AddShapeAction>(slideIndex_, std::move(removedShape_), false);
    }

    // CompositeAction - executes multiple actions as a single unit
    // Useful for operations like move that combine multiple actions
    class CompositeAction : public IAction {
        std::vector<std::unique_ptr<IAction>> actions_;

    public:
        void addAction(std::unique_ptr<IAction> action) {
            actions_.push_back(std::move(action));
        }

        std::unique_ptr<IAction> execute(Model::Presentation* presentation) override {
            if (!presentation || actions_.empty()) {
                return nullptr;
            }

            // Execute all actions in order and collect their converse actions
            std::vector<std::unique_ptr<IAction>> converseActions;
            for (auto& action : actions_) {
                std::unique_ptr<IAction> converse = action->execute(presentation);
                if (converse) {
                    converseActions.push_back(std::move(converse));
                }
            }

            // Create a composite converse action with actions in reverse order
            // If we did [A1, A2], to undo we need [converse(A2), converse(A1)]
            auto compositeConverse = std::make_unique<CompositeAction>();
            for (auto it = converseActions.rbegin(); it != converseActions.rend(); ++it) {
                compositeConverse->addAction(std::move(*it));
            }

            return compositeConverse;
        }
    };

} // namespace Application

