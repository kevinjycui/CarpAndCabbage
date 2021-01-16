#include "ChiliPepperMovementComponent.h"
#include <SFML/Window/Keyboard.hpp>   //<-- Add the keyboard include in order to get keyboard inputs

#include "GameEngine/GameEngineMain.h" //<-- Add this include to retrieve the delta time between frames
#include "../Socket.h"

using namespace Game;

void ChiliPepperMovementComponent::Update()
{
    __super::Update();

    //Grabs how much time has passed since last frame
    const float dt = GameEngine::GameEngineMain::GetTimeDelta();

    //By default the displacement is 0,0
    sf::Vector2f displacement{ 0.0f,0.0f };
    sf::Vector2f position = GetEntity()->GetPos();

    //The amount of speed that we will apply when input is received
    const float inputAmount = 300.0f;

    displacement.y += inputAmount * dt;

    //Update the entity position
    GetEntity()->SetPos(GetEntity()->GetPos() + displacement);
}

void ChiliPepperMovementComponent::OnAddToWorld() {
    __super::OnAddToWorld();
}

ChiliPepperMovementComponent::ChiliPepperMovementComponent() {}

ChiliPepperMovementComponent::~ChiliPepperMovementComponent() {}