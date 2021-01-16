#pragma once

#include "GameEngine/EntitySystem/Component.h"

namespace Game
{
    class KnifeMovementComponent : public GameEngine::Component
    {
    public:
        KnifeMovementComponent();
        ~KnifeMovementComponent();

        virtual void Update() override;
        virtual void OnAddToWorld() override;

        int direction;

    };
};
