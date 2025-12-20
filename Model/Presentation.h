#pragma once
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include "Slide.h"

namespace Model {

    class Presentation {
        std::vector<std::unique_ptr<Slide>> slides_;
        std::string title_;

    public:
        explicit Presentation(std::string title = "Untitled")
            : title_(std::move(title)) {
        }

        void addSlide(std::unique_ptr<Slide> slide, size_t position = static_cast<size_t>(-1)) {
            size_t defaultPos = static_cast<size_t>(-1);
            if (position == defaultPos) {

                slides_.push_back(std::move(slide));
            }
            else if (position > slides_.size()) {

                std::cout << "[WARNING] Given invalid position. Slide added at the end of presentation. You can use -remove_slide to delete it" << std::endl;
                slides_.push_back(std::move(slide));
            }
            else {
                slides_.insert(slides_.begin() + position, std::move(slide));
            }
        }

        void removeSlide(size_t index) {
            if (index >= slides_.size()) {
                throw std::out_of_range("Slide index out of range");
            }
            slides_.erase(slides_.begin() + index);
        }

        size_t slideCount() const {
            return slides_.size();
        }

        Slide* getSlide(size_t index) {
            if (index >= slides_.size()) {
                throw std::out_of_range("Slide index out of range");
            }
            return slides_[index].get();
        }

        const Slide* getSlide(size_t index) const {
            if (index >= slides_.size()) {
                throw std::out_of_range("Slide index out of range");
            }
            return slides_[index].get();
        }

        const std::vector<std::unique_ptr<Slide>>& getSlides() const {
            return slides_;
        }

        void save(const std::string& filename) const {
            std::ofstream out(filename);
            if (!out) {
                throw std::runtime_error("Cannot open file for writing: " + filename);
            }
            out << "PRESENTATION " << title_ << "\n";
            for (const auto& slide : slides_) {
                out << slide->serialize() << "\n";
            }
        }

        void load(const std::string& filename) {
            std::ifstream in(filename);
            if (!in) {
                throw std::runtime_error("Cannot open file for reading: " + filename);
            }

            slides_.clear();
            std::string line;

            if (std::getline(in, line) && line.rfind("PRESENTATION ", 0) == 0) {
                title_ = line.substr(13);
            }

            while (std::getline(in, line)) {
                if (line.rfind("SLIDE ", 0) == 0) {
                    slides_.push_back(std::make_unique<Slide>());
                }
            }
        }

        const std::string& title() const { return title_; }
        void setTitle(const std::string& title) { title_ = title; }
    };

}
