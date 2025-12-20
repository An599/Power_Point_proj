#pragma once
#include "../../Visualization/IBrush.h"
#include <string>

namespace Visualization {
    namespace SVG {
        class SVGBrush : public IBrush {
        private:
            std::string strokeColor_;
            std::string fillColor_;
            int strokeWidth_;
            float opacity_;
            std::string strokeStyle_;
            std::string fontFamily_;
            int fontSize_;

        public:
            SVGBrush(std::string strokeColor = "black",
                std::string fillColor = "none",
                int strokeWidth = 1,
                float opacity = 1.0f,
                std::string strokeStyle = "solid",
                std::string fontFamily = "Arial",
                int fontSize = 12)
                : strokeColor_(strokeColor),
                fillColor_(fillColor),
                strokeWidth_(strokeWidth),
                opacity_(opacity),
                strokeStyle_(strokeStyle),
                fontFamily_(fontFamily),
                fontSize_(fontSize) {
            }

            std::string getStrokeColor() const { return strokeColor_; }
            std::string getFillColor() const { return fillColor_; }
            int getStrokeWidth() const { return strokeWidth_; }
            float getOpacity() const { return opacity_; }
            std::string getStrokeStyle() const { return strokeStyle_; }
            std::string getFontFamily() const { return fontFamily_; }
            int getFontSize() const { return fontSize_; }

            std::string toSVGStyle() const {
                std::string style = "stroke:" + strokeColor_ + ";";
                style += "fill:" + fillColor_ + ";";
                style += "stroke-width:" + std::to_string(strokeWidth_) + ";";
                style += "opacity:" + std::to_string(opacity_) + ";";

                if (strokeStyle_ == "dashed") {
                    style += "stroke-dasharray:5,5;";
                }
                else if (strokeStyle_ == "dotted") {
                    style += "stroke-dasharray:2,2;";
                }

                return style;
            }
        };

    } // namespace SVG
} // namespace Visualization
