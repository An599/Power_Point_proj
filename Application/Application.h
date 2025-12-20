#pragma once
#include "Editor.h"

namespace Application {
    class Application {
    private:
        Editor editor_;

        Application() = default;
        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

    public:
        static Application& getInstance() {
            static Application instance;
            return instance;
        }

        Editor& getEditor() {
            return editor_;
        }

        const Editor& getEditor() const {
            return editor_;
        }
    };

} 

