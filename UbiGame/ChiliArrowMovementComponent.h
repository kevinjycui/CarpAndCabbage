#pragma once

#include "GameEngine/EntitySystem/Component.h"
#include <chrono>

namespace Game
{
    class ChiliArrowMovementComponent : public GameEngine::Component
    {
    public:
        ChiliArrowMovementComponent();
        ~ChiliArrowMovementComponent();

        virtual void Update() override;
        virtual void OnAddToWorld() override;

        std::chrono::steady_clock::time_point lastChiliDropTime;
    };
};
