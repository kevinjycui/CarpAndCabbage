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
#include <chrono>
#include <Game/GameBoard.h>

typedef std::chrono::high_resolution_clock Clock;

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
        auto now = Clock::now();
        std::cout << "here\n" << std::chrono::duration_cast<std::chrono::milliseconds>(now - lastChiliDropTime).count() << '\n';
        // 5 seconds before chili drop
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastChiliDropTime).count() >= 5000) {
            lastChiliDropTime = now;

            std::cout << "pog\n";

            nlohmann::json j;
            float x = GetEntity()->GetPos().x;
            std::string activatedById = Socket::playerId;
            j["x"] = x;
            j["activatedById"] = activatedById;

            Socket::io.socket()->emit("chiliAttack", j.dump());
            GameEngineMain::GetInstance()->m_gameBoard->SpawnPepper(x, activatedById);
        }
    }
}

void ChiliArrowMovementComponent::OnAddToWorld() {
    __super::OnAddToWorld();
}

ChiliArrowMovementComponent::ChiliArrowMovementComponent() {
    lastChiliDropTime = Clock::now();
}

ChiliArrowMovementComponent::~ChiliArrowMovementComponent() {}