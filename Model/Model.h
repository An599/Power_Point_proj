#pragma once
#include "Presentation.h"
#include <memory>

namespace Model {

    class Model {
    private:
        std::unique_ptr<Presentation> presentation_;

        Model() : presentation_(nullptr) {}

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

} 