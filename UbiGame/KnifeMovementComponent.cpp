#include "KnifeMovementComponent.h"
#include <SFML/Window/Keyboard.hpp>   //<-- Add the keyboard include in order to get keyboard inputs
#include <algorithm>
#include "GameEngine/GameEngineMain.h" //<-- Add this include to retrieve the delta time between frames

using namespace Game;

static float inputAmount = 300.0f;
static float increment = 0.01f;

void KnifeMovementComponent::Update()
{
    __super::Update();

    //Grabs how much time has passed since last frame
    const float dt = GameEngine::GameEngineMain::GetTimeDelta();

    //By default the displacement is 0,0
    sf::Vector2f displacement{ 0.0f,0.0f };
    sf::Vector2f position = GetEntity()->GetPos();

    //The amount of speed that we will apply when input is received

    inputAmount += increment;

    if (inputAmount > 900.f)
        increment = -0.01f;
    else if (inputAmount < 300.f)
        increment = 0.01f;

    displacement.x += inputAmount * dt * direction;

    if ((position + displacement).x <= 0 || (position + displacement).x > 1920) {
        direction *= -1;
        return;
    }

    //Update the entity position
    GetEntity()->SetPos(GetEntity()->GetPos() + displacement);
}

void KnifeMovementComponent::OnAddToWorld() {
    __super::OnAddToWorld();
    direction = 1;
}

KnifeMovementComponent::KnifeMovementComponent() {}

KnifeMovementComponent::~KnifeMovementComponent() {}