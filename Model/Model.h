#pragma once
#include "Model/Presentation.h"
#include <memory>

namespace Model {

    // Singleton Model - doesn't know about View or Controller
    class Model {
    private:
        std::unique_ptr<Presentation> presentation_;

        // Private constructor for Singleton
        Model() : presentation_(nullptr) {}

        // Delete copy constructor and assignment operator
        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;

    public:
        static Model& getInstance() {
            static Model instance;
            return instance;
        }

        Presentation* getPresentation() {
            return presentation_.get();
        }

        const Presentation* getPresentation() const {
            return presentation_.get();
        }

        void createPresentation(const std::string& title) {
            presentation_ = std::make_unique<Presentation>(title);
        }

        void setPresentation(std::unique_ptr<Presentation> pres) {
            presentation_ = std::move(pres);
        }

        bool hasPresentation() const {
            return presentation_ != nullptr;
        }
    };

} // namespace Model