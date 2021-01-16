#pragma once

#include "GameEngine/EntitySystem/Component.h"
#include <string>

namespace Game
{
    class PlayerMovementComponent : public GameEngine::Component
    {
    public:
        PlayerMovementComponent();
        ~PlayerMovementComponent();
        
        std::string playerId;
        void setPlayerId(std::string playerId);
        virtual void Update() override;
        virtual void OnAddToWorld() override;
    };
};
