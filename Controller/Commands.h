#pragma once
#include "ICommand.h"
#include "../Model/Model.h"
#include "../Model/Slide.h"
#include "../Model/Shapes.h"
#include "../Model/TextShape.h"
#include "../Viewer/View.h"
#include "../Serialization/JsonSerialize.h"
#include "../Serialization/JsonDeserialize.h"
#include "../Application/Application.h"
#include "../Application/Actions.h"
#include <memory>
#include <string>
#include <vector>

namespace Controller {
    class CreatePresentationCommand : public ICommand {
        std::string title_;

    public:
        explicit CreatePresentationCommand(std::string title = "Untitled Presentation")
            : title_(title) {
        }

        void execute() override {
            auto& model = Model::Model::getInstance();
            auto& view = View::ViewFacade::getInstance();
            model.createPresentation(title_);
            view.showSuccess("Created presentation: '" + title_ + "'");
        }

        void undo() override {}
        bool isUndoable() const override { return false; }
    };

    class LoadPresentationCommand : public ICommand {
        std::string filepath_;

    public:
        explicit LoadPresentationCommand(std::string filepath)
            : filepath_(filepath) {
        }

        void execute() override {
            auto& model = Model::Model::getInstance();
            auto& view = View::ViewFacade::getInstance();

            try {
                // Use JSON deserialize
                Serialization::JsonDeserialize loader;
                std::unique_ptr<Model::Presentation> pres = loader.load(filepath_);
                model.setPresentation(std::move(pres));
                view.showSuccess("Presentation loaded from '" + filepath_ + "'");
            }
            catch (const std::exception& e) {
                view.showError(std::string("Load failed: ") + e.what());
            }
        }

        void undo() override {}
        bool isUndoable() const override { return false; }
    };

    class SavePresentationCommand : public ICommand {
        std::string filename_;

    public:
        explicit SavePresentationCommand(std::string filename)
            : filename_(filename) {
        }

        void execute() override {
            auto& model = Model::Model::getInstance();
            auto& view = View::ViewFacade::getInstance();

            if (!model.hasPresentation()) {
                view.showError("No presentation to save. Create one first.");
                return;
            }

            try {
                // Use JSON serialize
                Serialization::JsonSerialize saver;
                saver.save(*model.getPresentation(), filename_);
                view.showSuccess("Presentation saved to '" + filename_ + "'");
            }
            catch (const std::exception& e) {
                view.showError(std::string("Save failed: ") + e.what());
            }
        }

        void undo() override {}
        bool isUndoable() const override { return false; }
    };

    class AddSlideCommand : public ICommand {
        size_t position_;
        bool hasPosition_;
        size_t actualPosition_;
        bool executed_;

    public:
        AddSlideCommand(size_t position, bool hasPosition)
            : position_(position), hasPosition_(hasPosition),
            actualPosition_(0), executed_(false) {
        }

        void execute() override {
            auto& model = Model::Model::getInstance();
            auto& view = View::ViewFacade::getInstance();
            auto& app = Application::Application::getInstance();

            if (!model.hasPresentation()) {
                view.showError("No presentation loaded.");
                executed_ = false;
                return;
            }

            // Create slide and action
            std::unique_ptr<Model::Slide> slide = std::make_unique<Model::Slide>();
            std::unique_ptr<Application::IAction> action = 
                std::make_unique<Application::AddSlideAction>(std::move(slide), position_, hasPosition_);

            // Execute action through Editor
            app.getEditor().doAction(std::move(action), model.getPresentation());

            view.showSuccess("Added slide");
            executed_ = true;
        }

        void undo() override {
            // Undo is handled by Editor, not directly by command
            // This method is kept for compatibility but undo should be done through Editor
        }

        bool isUndoable() const override { return executed_; }
    };

    class RemoveSlideCommand : public ICommand {
        size_t position_;
        std::unique_ptr<Model::Slide> removedSlide_;
        bool executed_;

    public:
        explicit RemoveSlideCommand(size_t position)
            : position_(position), executed_(false) {
        }

        void execute() override {
            auto& model = Model::Model::getInstance();
            auto& view = View::ViewFacade::getInstance();
            auto& app = Application::Application::getInstance();

            if (!model.hasPresentation()) {
                view.showError("No presentation loaded.");
                executed_ = false;
                return;
            }

            try {
                // Create remove action
                std::unique_ptr<Application::IAction> action = 
                    std::make_unique<Application::RemoveSlideAction>(position_);

                // Execute action through Editor
                app.getEditor().doAction(std::move(action), model.getPresentation());

                view.showSuccess("Removed slide at position " +
                    std::to_string(static_cast<long long>(position_)));
                executed_ = true;
            }
            catch (const std::exception& e) {
                view.showError(e.what());
                executed_ = false;
            }
        }

        void undo() override {
            // Undo is handled by Editor, not directly by command
            // This method is kept for compatibility but undo should be done through Editor
        }

        bool isUndoable() const override { return executed_; }
    };

    class AddShapeCommand : public ICommand {
        std::string shapeType_;
        int x_, y_, width_, height_;
        std::string color_;
        std::string fillColor_;
        std::string text_;
        size_t slideIndex_;
        bool toFront_;
        bool executed_;

    public:
        AddShapeCommand(std::string shapeType, int x, int y, int width, int height,
            std::string color, std::string fillColor, std::string text, size_t slideIndex, bool toFront = false)
            : shapeType_(shapeType), x_(x), y_(y), width_(width), height_(height),
            color_(color), fillColor_(fillColor), text_(text), slideIndex_(slideIndex),
            toFront_(toFront), executed_(false) {
        }

        void execute() override {
            auto& model = Model::Model::getInstance();
            auto& view = View::ViewFacade::getInstance();
            auto& app = Application::Application::getInstance();

            if (!model.hasPresentation()) {
                view.showError("No presentation loaded.");
                executed_ = false;
                return;
            }

            if (slideIndex_ >= model.getPresentation()->slideCount()) {
                view.showError("Slide index out of range");
                executed_ = false;
                return;
            }

            Model::BoundingBox bounds(x_, y_, width_, height_);
            std::unique_ptr<Model::IShape> shape;

            if (shapeType_ == "rectangle" || shapeType_ == "rect") {
                shape.reset(new Model::Rectangle(bounds, color_, fillColor_, text_));
            }
            else if (shapeType_ == "circle") {
                shape.reset(new Model::Circle(bounds, color_, fillColor_, text_));
            }
            else if (shapeType_ == "triangle") {
                shape.reset(new Model::Triangle(bounds, color_, fillColor_, text_));
            }
            else if (shapeType_ == "trapezoid") {
                shape.reset(new Model::Trapezoid(bounds, color_, fillColor_, text_));
            }
            else if (shapeType_ == "parallelogram") {
                shape.reset(new Model::Parallelogram(bounds, color_, fillColor_, text_));
            }
            else if (shapeType_ == "rhombus") {
                shape.reset(new Model::Rhombus(bounds, color_, fillColor_, text_));
            }
            else {
                view.showError("Unknown shape type: " + shapeType_);
                executed_ = false;
                return;
            }

            if (shape.get()) {
                // Create action and execute through Editor
                std::unique_ptr<Application::IAction> action =
                    std::make_unique<Application::AddShapeAction>(slideIndex_, std::move(shape), toFront_);

                // Execute action through Editor
                app.getEditor().doAction(std::move(action), model.getPresentation());

                view.showSuccess("Added " + shapeType_ + " to slide " +
                    std::to_string(static_cast<long long>(slideIndex_)));
                executed_ = true;
            }
        }

        void undo() override {
            // Undo is handled by Editor, not directly by command
        }

        bool isUndoable() const override { return executed_; }
    };

    class AddTextCommand : public ICommand {
        int x_, y_, width_, height_;
        std::string text_;
        std::string color_;
        size_t slideIndex_;
        bool toFront_;
        bool executed_;

    public:
        AddTextCommand(int x, int y, int width, int height, std::string text, std::string color, size_t slideIndex, bool toFront = false)
            : x_(x), y_(y), width_(width), height_(height), text_(text), color_(color), slideIndex_(slideIndex),
            toFront_(toFront), executed_(false) {
        }

        void execute() override {
            auto& model = Model::Model::getInstance();
            auto& view = View::ViewFacade::getInstance();
            auto& app = Application::Application::getInstance();

            if (!model.hasPresentation()) {
                view.showError("No presentation loaded.");
                executed_ = false;
                return;
            }

            if (slideIndex_ >= model.getPresentation()->slideCount()) {
                view.showError("Slide index out of range");
                executed_ = false;
                return;
            }

            // Create text shape (rectangle with invisible border, only text visible)
            Model::BoundingBox bounds(x_, y_, width_, height_);
            std::unique_ptr<Model::IShape> shape = std::make_unique<Model::TextShape>(bounds, text_, color_);

            if (shape.get()) {
                // Create action and execute through Editor
                std::unique_ptr<Application::IAction> action =
                    std::make_unique<Application::AddShapeAction>(slideIndex_, std::move(shape), toFront_);

                // Execute action through Editor
                app.getEditor().doAction(std::move(action), model.getPresentation());

                view.showSuccess("Added text to slide " +
                    std::to_string(static_cast<long long>(slideIndex_)));
                executed_ = true;
            }
        }

        void undo() override {
            // Undo is handled by Editor
        }

        bool isUndoable() const override { return executed_; }
    };

    class ShowCommand : public ICommand {
    public:
        void execute() override {
            auto& model = Model::Model::getInstance();
            auto& view = View::ViewFacade::getInstance();

            if (!model.hasPresentation()) {
                view.showError("No presentation loaded.");
                return;
            }

            const Model::Presentation* pres = model.getPresentation();
            std::vector<std::pair<std::string, std::vector<std::string>>> slidesData;

            for (size_t i = 0; i < pres->slideCount(); ++i) {
                const Model::Slide* slide = pres->getSlide(i);
                std::vector<std::string> shapes;

                // Get shapes in Z-order
                std::vector<Model::IShape*> sortedShapes = slide->getShapesByZOrder();
                for (size_t j = 0; j < sortedShapes.size(); ++j) {
                    shapes.push_back(sortedShapes[j]->getDescription());
                }

                slidesData.push_back(std::make_pair("", shapes));
            }

            view.showPresentation(pres->title(), slidesData);
        }

        void undo() override {}
        bool isUndoable() const override { return false; }
    };

    class UndoCommand : public ICommand {
    public:
        void execute() override;
        void undo() override {}
        bool isUndoable() const override { return false; }
    };

    class RedoCommand : public ICommand {
    public:
        void execute() override;
        void undo() override {}
        bool isUndoable() const override { return false; }
    };

    class DuplicateSlideCommand : public ICommand {
        size_t sourcePosition_;
        bool executed_;

    public:
        explicit DuplicateSlideCommand(size_t sourcePosition)
            : sourcePosition_(sourcePosition), executed_(false) {
        }

        void execute() override {
            auto& model = Model::Model::getInstance();
            auto& view = View::ViewFacade::getInstance();
            auto& app = Application::Application::getInstance();

            if (!model.hasPresentation()) {
                view.showError("No presentation loaded.");
                executed_ = false;
                return;
            }

            try {
                if (sourcePosition_ >= model.getPresentation()->slideCount()) {
                    view.showError("Source slide index out of range");
                    executed_ = false;
                    return;
                }

                // Clone the slide
                Model::Slide* sourceSlide = model.getPresentation()->getSlide(sourcePosition_);
                std::unique_ptr<Model::Slide> clonedSlide = sourceSlide->clone();

                // Create action to add the cloned slide (at end by default)
                std::unique_ptr<Application::IAction> action =
                    std::make_unique<Application::AddSlideAction>(std::move(clonedSlide));

                // Execute action through Editor
                app.getEditor().doAction(std::move(action), model.getPresentation());

                view.showSuccess("Duplicated slide from position " +
                    std::to_string(static_cast<long long>(sourcePosition_)));
                executed_ = true;
            }
            catch (const std::exception& e) {
                view.showError(e.what());
                executed_ = false;
            }
        }

        void undo() override {
            // Undo is handled by Editor
        }

        bool isUndoable() const override { return executed_; }
    };

    class MoveSlideCommand : public ICommand {
        size_t fromPosition_;
        size_t toPosition_;
        bool executed_;

    public:
        MoveSlideCommand(size_t fromPosition, size_t toPosition)
            : fromPosition_(fromPosition), toPosition_(toPosition), executed_(false) {
        }

        void execute() override {
            auto& model = Model::Model::getInstance();
            auto& view = View::ViewFacade::getInstance();
            auto& app = Application::Application::getInstance();

            if (!model.hasPresentation()) {
                view.showError("No presentation loaded.");
                executed_ = false;
                return;
            }

            try {
                size_t slideCount = model.getPresentation()->slideCount();
                if (fromPosition_ >= slideCount) {
                    view.showError("Source slide index out of range");
                    executed_ = false;
                    return;
                }

                // Validate toPosition
                if (toPosition_ > slideCount) {
                    view.showError("Target position out of range");
                    executed_ = false;
                    return;
                }

                // Calculate adjusted position for adding
                // If moving forward (from < to), we need to account for the removal
                size_t adjustedToPosition = toPosition_;
                if (fromPosition_ < toPosition_) {
                    adjustedToPosition = toPosition_ - 1; // Adjust because we remove first
                }
                // If moving backward (from > to), position stays the same

                // Clone the slide first (before any modifications)
                Model::Slide* sourceSlide = model.getPresentation()->getSlide(fromPosition_);
                std::unique_ptr<Model::Slide> clonedSlide = sourceSlide->clone();

                // Create composite action for move (remove + add)
                auto compositeAction = std::make_unique<Application::CompositeAction>();
                
                // First action: remove from original position
                compositeAction->addAction(std::make_unique<Application::RemoveSlideAction>(fromPosition_));
                
                // Second action: add at new position
                compositeAction->addAction(
                    std::make_unique<Application::AddSlideAction>(std::move(clonedSlide), adjustedToPosition, true));

                // Execute composite action through Editor
                app.getEditor().doAction(std::move(compositeAction), model.getPresentation());

                view.showSuccess("Moved slide from position " +
                    std::to_string(static_cast<long long>(fromPosition_)) + " to position " +
                    std::to_string(static_cast<long long>(adjustedToPosition)));
                executed_ = true;
            }
            catch (const std::exception& e) {
                view.showError(e.what());
                executed_ = false;
            }
        }

        void undo() override {
            // Undo is handled by Editor
        }

        bool isUndoable() const override { return executed_; }
    };

    class HelpCommand : public ICommand {
    public:
        void execute() override {
            auto& view = View::ViewFacade::getInstance();
            view.showHelp();
        }

        void undo() override {}
        bool isUndoable() const override { return false; }
    };

    class ExitCommand : public ICommand {
    public:
        void execute() override {
            auto& view = View::ViewFacade::getInstance();
            view.showInfo("Exiting program.");
            exit(0);
        }

        void undo() override {}
        bool isUndoable() const override { return false; }
    };

} 
