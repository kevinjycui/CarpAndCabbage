#pragma once

#include "GameEngine/EntitySystem/Component.h"
#include <string>

#include <chrono>

namespace Game
{
    class PlayerMovementComponent : public GameEngine::Component
    {
    public:
        PlayerMovementComponent();
        ~PlayerMovementComponent();

        virtual void Update() override;
        virtual void OnAddToWorld() override;

        float lastAnimation;
        int m_currentFaceIndex;

        std::chrono::steady_clock::time_point lastServerPositionUpdate;
    };
};
