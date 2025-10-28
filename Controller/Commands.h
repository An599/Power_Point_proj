#pragma once
#include "Controller/ICommand.h"
#include "Model/Model.h"
#include "Model/Slide.h"
#include "Model/Rectangle.h"
#include "Viewer/View.h"
#include <memory>
#include <string>
#include <vector>

namespace Controller {
    class CreatePresentationCommand : public ICommand {
        std::string title_;

    public:
        explicit CreatePresentationCommand(std::string title = "Untitled Presentation")
            : title_(std::move(title)) {
        }

        void execute() override {
            auto& model = Model::Model::getInstance();
            auto& view = View::ViewFacade::getInstance();

            model.createPresentation(title_);
            view.showSuccess("Created presentation: '" + title_ + "'");
        }
    };

    class LoadPresentationCommand : public ICommand {
        std::string filepath_;

    public:
        explicit LoadPresentationCommand(std::string filepath)
            : filepath_(std::move(filepath)) {
        }

        void execute() override {
            auto& model = Model::Model::getInstance();
            auto& view = View::ViewFacade::getInstance();

            try {
                auto pres = std::make_unique<Model::Presentation>();
                pres->load(filepath_);
                model.setPresentation(std::move(pres));
                view.showSuccess("Presentation loaded from '" + filepath_ + "'");
            }
            catch (const std::exception& e) {
                view.showError(e.what());
            }
        }
    };

    class SavePresentationCommand : public ICommand {
        std::string filename_;

    public:
        explicit SavePresentationCommand(std::string filename)
            : filename_(std::move(filename)) {
        }

        void execute() override {
            auto& model = Model::Model::getInstance();
            auto& view = View::ViewFacade::getInstance();

            if (!model.hasPresentation()) {
                view.showError("No presentation to save. Create one first.");
                return;
            }

            try {
                model.getPresentation()->save(filename_);
                view.showSuccess("Presentation saved to '" + filename_ + "'");
            }
            catch (const std::exception& e) {
                view.showError(e.what());
            }
        }
    };

    class AddSlideCommand : public ICommand {
        std::string title_;
        size_t position_;
        bool hasPosition_;

    public:
        AddSlideCommand(std::string title, size_t position, bool hasPosition)
            : title_(std::move(title)), position_(position), hasPosition_(hasPosition) {
        }

        void execute() override {
            auto& model = Model::Model::getInstance();
            auto& view = View::ViewFacade::getInstance();

            if (!model.hasPresentation()) {
                view.showError("No presentation loaded. Create one first with 'create_presentation'");
                return;
            }

            auto slide = std::make_unique<Model::Slide>(title_);
            if (hasPosition_) {
                model.getPresentation()->addSlide(std::move(slide), position_);
                view.showSuccess("Added slide '" + title_ + "' at position " + std::to_string(position_));
            }
            else {
                model.getPresentation()->addSlide(std::move(slide));
                view.showSuccess("Added slide '" + title_ + "' at end");
            }
        }
    };

    class RemoveSlideCommand : public ICommand {
        size_t position_;

    public:
        explicit RemoveSlideCommand(size_t position) : position_(position) {}

        void execute() override {
            auto& model = Model::Model::getInstance();
            auto& view = View::ViewFacade::getInstance();

            if (!model.hasPresentation()) {
                view.showError("No presentation loaded.");
                return;
            }

            if (model.getPresentation()->slideCount() == 0) {
                view.showWarning("No slides to remove");
                return;
            }

            try {
                model.getPresentation()->removeSlide(position_);
                view.showSuccess("Removed slide at position " + std::to_string(position_));
            }
            catch (const std::exception& e) {
                view.showError(e.what());
            }
        }
    };

    class AddShapeCommand : public ICommand {
        std::string shapeType_;
        int x_, y_, width_, height_;
        std::string color_;
        std::string text_;
        size_t slideIndex_;

    public:
        AddShapeCommand(std::string shapeType, int x, int y, int width, int height,
            std::string color, std::string text, size_t slideIndex)
            : shapeType_(std::move(shapeType)), x_(x), y_(y),
            width_(width), height_(height), color_(std::move(color)),
            text_(std::move(text)), slideIndex_(slideIndex) {
        }

        void execute() override {
            auto& model = Model::Model::getInstance();
            auto& view = View::ViewFacade::getInstance();

            if (!model.hasPresentation()) {
                view.showError("No presentation loaded.");
                return;
            }

            if (slideIndex_ >= model.getPresentation()->slideCount()) {
                view.showError("Slide index out of range");
                return;
            }

            Model::Slide* slide = model.getPresentation()->getSlide(slideIndex_);

            if (shapeType_ == "rectangle" || shapeType_ == "rect") {
                slide->addShape(std::make_unique<Model::Rectangle>(x_, y_, width_, height_, color_, text_));
                view.showSuccess("Added rectangle to slide " + std::to_string(slideIndex_));
            }
            else if (shapeType_ == "circle") {
                // For circle, width is used as radius
                slide->addShape(std::make_unique<Model::Circle>(x_, y_, width_, color_, text_));
                view.showSuccess("Added circle to slide " + std::to_string(slideIndex_));
            }
            else if (shapeType_ == "triangle") {
                // For triangle, use x,y as first point, width/height to calculate other points
                int x2 = x_ + width_;
                int y2 = y_;
                int x3 = x_ + width_ / 2;
                int y3 = y_ + height_;
                slide->addShape(std::make_unique<Model::Triangle>(x_, y_, x2, y2, x3, y3, color_, text_));
                view.showSuccess("Added triangle to slide " + std::to_string(slideIndex_));
            }
            else {
                view.showError("Unknown shape type: " + shapeType_ + ". Use rectangle, circle, or triangle.");
            }
        }
    };

    class ShowCommand : public ICommand {
    public:
        void execute() override {
            auto& model = Model::Model::getInstance();
            auto& view = View::ViewFacade::getInstance();

            if (!model.hasPresentation()) {
                view.showError("No presentation loaded. Create one with 'create_presentation <title>'");
                return;
            }

            const auto* pres = model.getPresentation();
            std::vector<std::pair<std::string, std::vector<std::string>>> slidesData;

            for (size_t i = 0; i < pres->slideCount(); ++i) {
                const auto* slide = pres->getSlide(i);
                std::vector<std::string> shapes;
                for (const auto& shape : slide->getShapes()) {
                    shapes.push_back(shape->getDescription());
                }
                slidesData.emplace_back(slide->title(), shapes);
            }

            view.showPresentation(pres->title(), slidesData);
        }
    };

    class HelpCommand : public ICommand {
    public:
        void execute() override {
            auto& view = View::ViewFacade::getInstance();
            view.showHelp();
        }
    };

    class ExitCommand : public ICommand {
    public:
        void execute() override {
            auto& view = View::ViewFacade::getInstance();
            view.showInfo("Exiting program.");
            exit(0);
        }
    };

} 
