#pragma once
#include "Editor.h"

namespace Application {

    // Application class - holds Editor instance
    // Part of Application layer, coordinates between Model, View, and Controller
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

} // namespace Application

