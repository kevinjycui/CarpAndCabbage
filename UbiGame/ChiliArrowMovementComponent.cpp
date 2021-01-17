#include "ChiliArrowMovementComponent.h"
#include <SFML/Window/Keyboard.hpp>   //<-- Add the keyboard include in order to get keyboard inputs

#include "GameEngine/GameEngineMain.h" //<-- Add this include to retrieve the delta time between frames
#include "json.hpp"
#include "../Socket.h"
#include <iostream>
#include <string>
#include <GameEngine/EntitySystem/Components/SpriteRenderComponent.h>

#include "GameEngine/EntitySystem/Components/CollidableComponent.h"
#include "GameEngine/Util/CollisionManager.h"

#include <Game/GameBoard.h>
using namespace GameEngine;
using namespace Game;
using sio::socket;
using sio::message;

static float velocity_y = 0.f;
static float acceleration_y = 0.098f;

void ChiliArrowMovementComponent::Update()
{
    __super::Update();

    //Grabs how much time has passed since last frame
    const float dt = GameEngine::GameEngineMain::GetTimeDelta();

    //By default the displacement is 0,0
    sf::Vector2f displacement{ 0.0f,0.0f };
    sf::Vector2f position = GetEntity()->GetPos();

    //The amount of speed that we will apply when input is received
    const float inputAmount = 300.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        displacement.x -= inputAmount * dt;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        displacement.x += inputAmount * dt;
    }

    // Prevent from going out of bounds
    if (Socket::isFish) {
        if ((position + displacement).x < 960 || (position + displacement).x > 1920) {
            return;
        }
    }
    else {
        if ((position + displacement).x < 0 || (position + displacement).x > 960) {
            return;
        }
    }

    sf::Vector2f newPos = GetEntity()->GetPos() + displacement;
    GetEntity()->SetPos(newPos);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        // 5 second cooldown
        if (difftime(std::time(nullptr), lastActivated) <= 5) {
            return;
        }
        lastActivated = std::time(nullptr);

        nlohmann::json j;
        j["x"] = GetEntity()->GetPos().x;
        j["activatedById"] = Socket::playerId;
        Socket::io.socket()->emit("chiliAttack", j.dump());
    }
}

void ChiliArrowMovementComponent::OnAddToWorld() {
    __super::OnAddToWorld();
}

ChiliArrowMovementComponent::ChiliArrowMovementComponent() {
    lastActivated = std::time(nullptr);
}

ChiliArrowMovementComponent::~ChiliArrowMovementComponent() {}