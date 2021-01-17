#include "ChiliPepperMovementComponent.h"
#include <SFML/Window/Keyboard.hpp>   //<-- Add the keyboard include in order to get keyboard inputs

#include "GameEngine/GameEngineMain.h" //<-- Add this include to retrieve the delta time between frames
#include "../Socket.h"
#include "json.hpp"
#include <ctime>
#include <iostream>
#include <string>
#include <SFML/Graphics/Rect.hpp>
#include "GameEngine/EntitySystem/Components/CollidableComponent.h"
#include <GameEngine/Util/CollisionManager.h>

#include <GameEngine/EntitySystem/Components/SpriteRenderComponent.h>
#include <GameEngine/EntitySystem/Components/ParticleEmitterComponent.h>

#include "GameEngine/EntitySystem/Components/CollidableComponent.h"
#include "GameEngine/Util/CollisionManager.h"

#include <Game/GameBoard.h>
#include "KnifeMovementComponent.h"

#include <chrono>

typedef std::chrono::high_resolution_clock Clock;

using namespace GameEngine;
using namespace Game;

typedef sf::Rect<float> AABBRect;

float acceleration_y = 0.098f;

void ChiliPepperMovementComponent::Update()
{
    __super::Update();

    //Grabs how much time has passed since last frame
    const float dt = GameEngine::GameEngineMain::GetTimeDelta();

    //By default the displacement is 0,0
    sf::Vector2f displacement{ 0.0f,0.1f };
    sf::Vector2f position = GetEntity()->GetPos();

    //The amount of speed that we will apply when input is received
    const float inputAmount = 300.0f;

    velocity_y += acceleration_y;

    displacement.x += inputAmount * dt * velocity_x;
    displacement.y += inputAmount * dt * velocity_y;

    //Update the entity position
    GetEntity()->SetPos(GetEntity()->GetPos() + displacement);

    std::vector<CollidableComponent*>& collidables = CollisionManager::GetInstance()->GetCollidables();

    auto size = this->GetEntity()->GetSize();

    for (int a = 0; a < collidables.size(); ++a)
    {
        CollidableComponent* colComponent = collidables[a];
        if (colComponent->GetEntity() == this->GetEntity())
            continue;

        AABBRect intersection;
        AABBRect myBox = AABBRect(position, size);

        AABBRect collideBox = AABBRect(
            colComponent->GetEntity()->GetPos(), colComponent->GetEntity()->GetSize()); //colComponent->GetWorldAABB();
            // Collides A
            //          B
            // a.k.a. user has landed on a platform

        if (myBox.top + 1.9 * collideBox.height >= collideBox.top &&
            myBox.top <= collideBox.top + collideBox.height &&
            myBox.left <= collideBox.left + myBox.width &&
            myBox.left + myBox.width >= collideBox.left) {

            velocity_y = -3.f;

            if (myBox.left <= collideBox.left + myBox.width / 2)
                velocity_x = -1.f;
            else
                velocity_x = 1.f;

        }

        float xPos = GetEntity()->GetPos().x;

    }

}

void ChiliPepperMovementComponent::OnAddToWorld() {
    __super::OnAddToWorld();
    velocity_y = 0.f;
    velocity_x = 0.f;
}

ChiliPepperMovementComponent::ChiliPepperMovementComponent() {
}

ChiliPepperMovementComponent::~ChiliPepperMovementComponent() {}