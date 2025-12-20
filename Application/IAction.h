#pragma once
#include "../Model/Presentation.h"
#include <memory>

namespace Application {

    class IAction {
    public:
        virtual ~IAction() = default;

        virtual std::unique_ptr<IAction> execute(Model::Presentation* presentation) = 0;
    };

}
