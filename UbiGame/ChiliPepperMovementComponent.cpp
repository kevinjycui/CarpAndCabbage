#include "ChiliPepperMovementComponent.h"
#include <SFML/Window/Keyboard.hpp>   //<-- Add the keyboard include in order to get keyboard inputs

#include "GameEngine/GameEngineMain.h" //<-- Add this include to retrieve the delta time between frames
#include "../Socket.h"
#include <ctime>
#include <SFML/Graphics/Rect.hpp>


using namespace Game;

typedef sf::Rect<float> AABBRect;

void ChiliPepperMovementComponent::Update()
{
    __super::Update();

    //Grabs how much time has passed since last frame
    const float dt = GameEngine::GameEngineMain::GetTimeDelta();

    //By default the displacement is 0,0
    sf::Vector2f displacement{ 0.0f,1.0f };
    sf::Vector2f position = GetEntity()->GetPos();

    //The amount of speed that we will apply when input is received
    const float inputAmount = 300.0f;

    displacement.y += inputAmount * dt;

    //Update the entity position
    GetEntity()->SetPos(GetEntity()->GetPos() + displacement);

    auto playerPosition = GameEngine::GameEngineMain::GetInstance()->m_gameBoard->player->GetPos();
    auto playerSize = GameEngine::GameEngineMain::GetInstance()->m_gameBoard->player->GetSize();

    auto chiliPosition = GetEntity()->GetPos();
    auto chiliSize = GetEntity()->GetSize();

    AABBRect playerRect = AABBRect(playerPosition, playerSize);
    AABBRect chiliRect = AABBRect(chiliPosition, chiliSize);

    if (playerRect.intersects(chiliRect)) {
        Socket::isGameOver = true;
    }
}

void ChiliPepperMovementComponent::OnAddToWorld() {
    __super::OnAddToWorld();
}

ChiliPepperMovementComponent::ChiliPepperMovementComponent() {

}

ChiliPepperMovementComponent::~ChiliPepperMovementComponent() {}