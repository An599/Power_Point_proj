#pragma once
#include "IAction.h"
#include "../Model/Slide.h"
#include "../Model/IShape.h"
#include <memory>
#include <string>
#include <vector>

namespace Application {

    class RemoveSlideAction;
    class RemoveShapeAction;
    class AddShapeAction;

    class RemoveSlideAction : public IAction {
        size_t position_;
        std::unique_ptr<Model::Slide> removedSlide_;

    public:
        explicit RemoveSlideAction(size_t position) : position_(position) {
        }

        std::unique_ptr<IAction> execute(Model::Presentation* presentation) override;
    };

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

            return std::make_unique<RemoveSlideAction>(actualPosition);
        }
    };

    inline std::unique_ptr<IAction> RemoveSlideAction::execute(Model::Presentation* presentation) {
        if (!presentation || position_ >= presentation->slideCount()) {
            return nullptr;
        }

        Model::Slide* slidePtr = presentation->getSlide(position_);
        removedSlide_ = slidePtr->clone();

        presentation->removeSlide(position_);

        return std::make_unique<AddSlideAction>(std::move(removedSlide_), position_, true);
    }

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

    class AddShapeAction : public IAction {
        size_t slideIndex_;
        std::unique_ptr<Model::IShape> shape_;
        bool toFront_;
        size_t actualShapeIndex_;

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

            slide->addShape(std::move(shape_));

            actualShapeIndex_ = originalCount;

            if (toFront_ && actualShapeIndex_ < slide->shapeCount()) {
                slide->bringToFront(actualShapeIndex_);
                actualShapeIndex_ = slide->shapeCount() - 1;
            }

            return std::make_unique<RemoveShapeAction>(slideIndex_, actualShapeIndex_);
        }
    };

    inline std::unique_ptr<IAction> RemoveShapeAction::execute(Model::Presentation* presentation) {
        if (!presentation || slideIndex_ >= presentation->slideCount()) {
            return nullptr;
        }

        Model::Slide* slide = presentation->getSlide(slideIndex_);
        if (shapeIndex_ >= slide->shapeCount()) {
            return nullptr;
        }

        Model::IShape* shapePtr = slide->getShapes()[shapeIndex_].get();
        removedShape_ = shapePtr->clone();

        slide->removeShapeAt(shapeIndex_);

        return std::make_unique<AddShapeAction>(slideIndex_, std::move(removedShape_), false);
    }

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

            std::vector<std::unique_ptr<IAction>> converseActions;
            for (auto& action : actions_) {
                std::unique_ptr<IAction> converse = action->execute(presentation);
                if (converse) {
                    converseActions.push_back(std::move(converse));
                }
            }

            auto compositeConverse = std::make_unique<CompositeAction>();
            for (auto it = converseActions.rbegin(); it != converseActions.rend(); ++it) {
                compositeConverse->addAction(std::move(*it));
            }

            return compositeConverse;
        }
    };

}
