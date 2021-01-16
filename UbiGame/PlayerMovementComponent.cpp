#include "PlayerMovementComponent.h"
#include <SFML/Window/Keyboard.hpp>   //<-- Add the keyboard include in order to get keyboard inputs

#include "GameEngine/GameEngineMain.h" //<-- Add this include to retrieve the delta time between frames
#include "json.hpp"
#include "../Socket.h"
#include <iostream>
#include <string>

using namespace Game;
using sio::socket;
using sio::message;

void PlayerMovementComponent::Update()
{
    __super::Update();

    //Grabs how much time has passed since last frame
    const float dt = GameEngine::GameEngineMain::GetTimeDelta();

    //By default the displacement is 0,0
    sf::Vector2f displacement{ 0.0f,0.0f };
    sf::Vector2f position = GetEntity()->GetPos();

    //The amount of speed that we will apply when input is received
    const float inputAmount = 300.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        displacement.x -= inputAmount * dt;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        displacement.x += inputAmount * dt;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        displacement.y -= inputAmount * dt * 2;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        displacement.y += inputAmount * dt;
        GetEntity()->SetSize(sf::Vector2f(128.0f, 64.0f));
    }
    else GetEntity()->SetSize(sf::Vector2f(128.0f, 128.0f));

    if (Socket::isFish) {
        if ((position + displacement).x <= GetEntity()->GetSize().y / 2 ||
            (position + displacement).x > 1920 / 2 - GetEntity()->GetSize().y / 2 ||
            (position + displacement).y <= GetEntity()->GetSize().y / 2 ||
            (position + displacement).y > 1080 - GetEntity()->GetSize().y / 2) {
                if (position.x > 1920 / 2 - GetEntity()->GetSize().y / 2) {
                    sf::Vector2f fix{ 1920 / 2 - GetEntity()->GetSize().y / 2, GetEntity()->GetPos().y };
                    GetEntity()->SetPos(fix);
                }
                return;
        }
    }
    else {
        if ((position + displacement).x > 1920 - GetEntity()->GetSize().y / 2 ||
            (position + displacement).x <= 1920 / 2 + GetEntity()->GetSize().y / 2 ||
            (position + displacement).y <= GetEntity()->GetSize().y / 2 ||
            (position + displacement).y > 1080 - GetEntity()->GetSize().y / 2) {
                if (position.x <= 1920 / 2 + GetEntity()->GetSize().y / 2) {
                    sf::Vector2f fix{ 1920 / 2 + GetEntity()->GetSize().y / 2 + 1, GetEntity()->GetPos().y };
                    GetEntity()->SetPos(fix);
                }
                return;
        }
    }


    // Update the entity position locally
    GetEntity()->SetPos(GetEntity()->GetPos() + displacement);

    sf::Vector2f gravity{ 0.0f, 1.0f };
    GetEntity()->SetPos(GetEntity()->GetPos() + gravity);

    // Only send update to server when user has moved
    nlohmann::json j;
    j["x"] = GetEntity()->GetPos().x;
    j["y"] = GetEntity()->GetPos().y;
    Socket::io.socket()->emit("movePlayer", j.dump());
}

void PlayerMovementComponent::OnAddToWorld() {
    __super::OnAddToWorld();
}

PlayerMovementComponent::PlayerMovementComponent(){}

PlayerMovementComponent::~PlayerMovementComponent() {}