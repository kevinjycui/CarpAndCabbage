#pragma once

#include "GameEngine/EntitySystem/Component.h"
#include <ctime>

namespace Game
{
    class ChiliArrowMovementComponent : public GameEngine::Component
    {
    public:
        ChiliArrowMovementComponent();
        ~ChiliArrowMovementComponent();

        virtual void Update() override;
        virtual void OnAddToWorld() override;

        std::time_t lastActivated;
    };
};
