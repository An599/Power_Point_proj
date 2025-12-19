#pragma once
#include "ICommand.h"
#include "../Model/Model.h"
#include "../Viewer/View.h"
#include "../Painting/SVGCanvas.h"
#include "../Painting/SVGPainter.h"
#include "../Painting/TransformedPainter.h"
#include <memory>
#include <fstream>

namespace Controller {

    // RenderCommand - renders presentation using visitor pattern
    // Shapes are drawn in Z-order (back to front)
    class RenderCommand : public ICommand {
        std::string outputPath_;
        int slideIndex_;

    public:
        RenderCommand(std::string outputPath, int slideIndex = -1)
            : outputPath_(outputPath), slideIndex_(slideIndex) {
        }

        void execute() override {
            auto& model = Model::Model::getInstance();
            auto& view = View::ViewFacade::getInstance();

            if (!model.hasPresentation()) {
                view.showError("No presentation loaded. Create one first.");
                return;
            }

            const Model::Presentation* pres = model.getPresentation();

            if (pres->slideCount() == 0) {
                view.showError("Presentation has no slides. Add slides first.");
                return;
            }

            // Determine which slides to render
            size_t startSlide = 0;
            size_t endSlide = pres->slideCount();

            if (slideIndex_ >= 0) {
                if (slideIndex_ >= static_cast<int>(pres->slideCount())) {
                    view.showError("Slide index out of range");
                    return;
                }
                startSlide = static_cast<size_t>(slideIndex_);
                endSlide = startSlide + 1;
            }

            // Calculate canvas height
            int slideHeight = 250;
            int canvasHeight = 100 + static_cast<int>((endSlide - startSlide) * slideHeight);
            if (canvasHeight < 600) canvasHeight = 600;
            if (canvasHeight > 4000) canvasHeight = 4000;

            // Create SVG canvas
            Painting::SVGCanvas canvas(800, canvasHeight);
            Painting::SVGPainter& painter = canvas.getPainter();

            canvas.beginDrawing();

            // Left margin (tab = 4 spaces ≈ 60 pixels for consistency)
            const int leftMargin = 60;
            
            // Draw presentation title with proper left margin
            painter.drawTextLeft(leftMargin, 40, pres->title(), "Arial", 24, "black");
            
            // Draw separator line below title (with margins)
            Painting::Pen titleSeparatorPen("gray", 2, Painting::Pen::Type::SOLID);
            painter.drawLine(leftMargin, 60, 800 - leftMargin, 60, titleSeparatorPen);

            // Render each slide with more spacing
            int yOffset = 90;
            for (size_t i = startSlide; i < endSlide; ++i) {
                const Model::Slide* slide = pres->getSlide(i);

                // Draw slide separator (with margins)
                if (i > startSlide) {
                    Painting::Pen separatorPen("lightgray", 1, Painting::Pen::Type::DASHED);
                    painter.drawLine(leftMargin, yOffset - 10, 800 - leftMargin, yOffset - 10, separatorPen);
                }

                // Draw slide number (left-aligned with proper margin)
                std::string slideTitle = "Slide " + std::to_string(static_cast<long long>(i));
                painter.drawTextLeft(leftMargin, yOffset, slideTitle, "Arial", 18, "blue");
                yOffset += 30;

                // Draw slide background area (starts at leftMargin to maintain proper indentation)
                int slideX = leftMargin;
                int slideY = yOffset;
                int slideWidth = 800 - 2 * leftMargin;
                int slideHeightArea = 200;
                Painting::Pen bgPen("lightgray", 1, Painting::Pen::Type::SOLID);
                Painting::Brush bgBrush("white", Painting::Brush::Style::SOLID);
                int bgXPoints[4] = { slideX, slideX + slideWidth, slideX + slideWidth, slideX };
                int bgYPoints[4] = { slideY, slideY, slideY + slideHeightArea, slideY + slideHeightArea };
                painter.drawPolygon(bgXPoints, bgYPoints, 4, bgPen, bgBrush);

                // Add padding inside slide area to prevent text/shapes from being cut off
                // Use larger padding to account for text-anchor="middle" which extends text on both sides
                int padding = 20;
                int contentX = slideX + padding;
                int contentY = slideY + padding;
                int contentWidth = slideWidth - 2 * padding;
                int contentHeight = slideHeightArea - 2 * padding;

                // Get shapes sorted by Z-order
                std::vector<Model::IShape*> sortedShapes = slide->getShapesByZOrder();

                if (sortedShapes.empty()) {
                    // Use left-aligned text for empty slide message
                    painter.drawTextLeft(
                        slideX + padding, slideY + slideHeightArea / 2, 
                        "(No shapes on this slide)", "Arial", 12, "gray");
                    yOffset += 30;
                }
                else {
                    // Create transformed painter that offsets shapes into slide area with padding
                    Painting::TransformedPainter transformedPainter(painter, contentX, contentY);
                    
                    // Draw each shape in Z-order (back to front)
                    for (size_t j = 0; j < sortedShapes.size(); ++j) {
                        sortedShapes[j]->draw(transformedPainter);
                    }
                    yOffset += 200;
                }

                yOffset += 20;
            }

            canvas.endDrawing();

            // Write to file
            std::string svg = canvas.getOutput();
            std::ofstream file(outputPath_.c_str());
            if (!file) {
                view.showError("Cannot write to file: " + outputPath_);
                return;
            }

            file << svg;
            file.close();

            if (slideIndex_ >= 0) {
                view.showSuccess("Rendered slide " +
                    std::to_string(static_cast<long long>(slideIndex_)) +
                    " to '" + outputPath_ + "'");
            }
            else {
                view.showSuccess("Rendered " +
                    std::to_string(static_cast<long long>(endSlide - startSlide)) +
                    " slide(s) to '" + outputPath_ + "'");
            }
        }

        void undo() override {}
        bool isUndoable() const override { return false; }
    };

    // Factory for render command
    class RenderFactory : public ICommandFactory {
    public:
        std::unique_ptr<ICommand> createCommand(const std::vector<std::string>& args) override {
            // Default output path if not provided
            std::string outputPath = "output.svg";
            if (args.size() >= 2 && !args[1].empty()) {
                outputPath = args[1];
            }
            
            int slideIndex = -1;

            for (size_t i = 2; i < args.size(); ++i) {
                if (args[i] == "-slide" && i + 1 < args.size()) {
                    slideIndex = std::stoi(args[i + 1]);
                    break;
                }
            }

            return std::unique_ptr<ICommand>(new RenderCommand(outputPath, slideIndex));
        }

        std::string getCommandName() const override {
            return "render";
        }
    };

} // namespace Controller