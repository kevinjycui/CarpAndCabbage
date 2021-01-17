#pragma once

#include "GameEngine/EntitySystem/Component.h"
#include <GameEngine/EntitySystem/Components/SoundComponent.h>

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

        static GameEngine::SoundComponent* chiliSoundCompon;
        static int fireSoundId;

        std::chrono::steady_clock::time_point lastChiliDropTime;
    };
};
