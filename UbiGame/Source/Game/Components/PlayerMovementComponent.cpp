#include "PlayerMovementComponent.h"
#include <SFML/Window/Keyboard.hpp>   //<-- Add the keyboard include in order to get keyboard inputs

#include "GameEngine/GameEngineMain.h" //<-- Add this include to retrieve the delta time between frames

using namespace Game;

void PlayerMovementComponent::Update()
{
    __super::Update();
    float playerVelocity = 300.f;
    float jumpVelocity = 1000.f;
    sf::Vector2f wantedVelocity = sf:Vector2f{ 0.0f,0.0f };

    //Grabs how much time has passed since last frame
    const float dt = GameEngine::GameEngineMain::GetTimeDelta();


    bool wasd = position.x <= 1920.0f / 2;

    switch (wasd) {
    case true:
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            wantedVelocity.x -= playerVelocity;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            wantedVelocity.x -= playerVelocity;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            if (!m_wasJumpButtonPressed)
            {
                wantevdVelocity.y -= jumpVelocity;
                m_wasJumpButtonPressed = True;
            }
        }
        else {
            m_wasJumpButtonPressed = False;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            displacement.y += inputAmount * dt;
        }
        if ((position + displacement).x <= GetEntity()->GetSize().y / 2 ||
            (position + displacement).x > 1920 / 2 - GetEntity()->GetSize().y ||
            (position + displacement).y <= GetEntity()->GetSize().y / 2 ||
            (position + displacement).y > 1080 - GetEntity()->GetSize().y)
            return;
        break;
    case false:
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            displacement.x -= inputAmount * dt;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            displacement.x += inputAmount * dt;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            displacement.y -= inputAmount * dt;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            displacement.y += inputAmount * dt;
        }
        if ((position + displacement).x > 1920 - GetEntity()->GetSize().y / 2 ||
            (position + displacement).x <= 1920 / 2 ||
            (position + displacement).y <= GetEntity()->GetSize().y / 2 ||
            (position + displacement).y > 1080 - GetEntity()->GetSize().y / 2)
            return;
        break;
    }

    //Update the entity position
    GetEntity()->SetPos(GetEntity()->GetPos() + displacement);

}

void PlayerMovementComponent::OnAddToWorld() {
    __super::OnAddToWorld();
}

PlayerMovementComponent::PlayerMovementComponent(){}

PlayerMovementComponent::~PlayerMovementComponent() {}