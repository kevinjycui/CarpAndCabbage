#pragma once

#include "GameEngine/EntitySystem/Component.h"

namespace Game
{
    class ChiliArrowMovementComponent : public GameEngine::Component
    {
    public:
        ChiliArrowMovementComponent();
        ~ChiliArrowMovementComponent();

        virtual void Update() override;
        virtual void OnAddToWorld() override;
    };
};
