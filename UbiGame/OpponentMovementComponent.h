#pragma once

#include "GameEngine/EntitySystem/Component.h"

namespace Game
{
    class OpponentMovementComponent : public GameEngine::Component
    {
    public:
        OpponentMovementComponent();
        ~OpponentMovementComponent();

        virtual void Update() override;
        virtual void OnAddToWorld() override;

    };
};
#pragma once
