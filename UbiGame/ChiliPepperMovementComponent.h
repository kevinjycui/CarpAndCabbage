#pragma once

#include "GameEngine/EntitySystem/Component.h"


namespace Game
{
    class ChiliPepperMovementComponent : public GameEngine::Component
    {
    public:
        ChiliPepperMovementComponent();
        ~ChiliPepperMovementComponent();

        virtual void Update() override;
        virtual void OnAddToWorld() override;

        float velocity_y;
        float velocity_x;

    };
};
