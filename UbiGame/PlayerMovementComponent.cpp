#include "PlayerMovementComponent.h"
#include <SFML/Window/Keyboard.hpp>   //<-- Add the keyboard include in order to get keyboard inputs

#include "GameEngine/GameEngineMain.h" //<-- Add this include to retrieve the delta time between frames
#include "json.hpp"
#include "../Socket.h"
#include <iostream>

using namespace Game;

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

    bool wasd = position.x <= 1920.0f / 2 + GetEntity()->GetSize().y / 2;

    switch (wasd) {
    case true:
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            displacement.x -= inputAmount * dt;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            displacement.x += inputAmount * dt;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            displacement.y -= inputAmount * dt;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            displacement.y += inputAmount * dt;
        }
        if ((position + displacement).x <= GetEntity()->GetSize().y / 2 ||
            (position + displacement).x > 1920 / 2 - GetEntity()->GetSize().y / 2 ||
            (position + displacement).y <= GetEntity()->GetSize().y / 2 ||
            (position + displacement).y > 1080 - GetEntity()->GetSize().y / 2)
            return;
        break;
    case false:
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
        {
            displacement.x -= inputAmount * dt;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
        {
            displacement.x += inputAmount * dt;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
        {
            displacement.y -= inputAmount * dt;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
        {
            displacement.y += inputAmount * dt;
        }
        if ((position + displacement).x > 1920 - GetEntity()->GetSize().y / 2 ||
            (position + displacement).x <= 1920 / 2 + GetEntity()->GetSize().y / 2 ||
            (position + displacement).y <= GetEntity()->GetSize().y / 2 ||
            (position + displacement).y > 1080 - GetEntity()->GetSize().y / 2)
            return;
        break;
    }

    // Update the entity position locally
    GetEntity()->SetPos(GetEntity()->GetPos() + displacement);
    
    // Only send update to server when user has moved
    if (displacement.x != 0 || displacement.y != 0) {
        nlohmann::json j;
        j["x"] = GetEntity()->GetPos().x;
        j["y"] = GetEntity()->GetPos().y;
        Socket::io.socket()->emit("movePlayer", j.dump());
    }
}

void PlayerMovementComponent::OnAddToWorld() {
    __super::OnAddToWorld();
}

PlayerMovementComponent::PlayerMovementComponent(){
    Socket::io.socket()->on("movePlayer", [&](sio::event& ev) {
        std::cout << "hello";
        std::cout << ev.get_message();
    });
}

PlayerMovementComponent::~PlayerMovementComponent() {}