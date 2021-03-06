#include "PlayerMovementComponent.h"
#include <SFML/Window/Keyboard.hpp>   //<-- Add the keyboard include in order to get keyboard inputs

#include "GameEngine/GameEngineMain.h" //<-- Add this include to retrieve the delta time between frames
#include "json.hpp"
#include "../Socket.h"
#include <iostream>
#include <string>
#include <ctime>
#include <stdlib.h>
#include <GameEngine/EntitySystem/Components/SpriteRenderComponent.h>

#include "GameEngine/EntitySystem/Components/CollidableComponent.h"
#include "GameEngine/Util/CollisionManager.h"

#include <Game/GameBoard.h>
#include "KnifeMovementComponent.h"

#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

using namespace GameEngine;
using namespace Game;
using sio::socket;
using sio::message;

static float velocity_y = 0.f;
static float acceleration_y = 0.098f;

static float velocity_x = 0.f;

void PlayerMovementComponent::Update()
{
    __super::Update();

    if (GetEntity()->GetPos().y >= 1920) {
        Socket::io.socket()->emit("playerDied");
        if (Socket::isFish) {
            Socket::isFishDead = true;
        }
        else if (!Socket::isFish) {
            Socket::isCabbageDead = true;
        }
    }

    if (Socket::isFishDead || Socket::isCabbageDead) {
        return;
    }
    
    velocity_y += acceleration_y;

    GameEngine::SpriteRenderComponent* playerSprite = GetEntity()->GetComponent<GameEngine::SpriteRenderComponent>();

    //Grabs how much time has passed since last frame
    const float dt = GameEngine::GameEngineMain::GetTimeDelta();

    std::cout << dt << '\n';

    lastAnimation += dt;

    int maxFaces = 3;
    if (Socket::isFish && lastAnimation > 1.f)
    {
        lastAnimation = 0;
        ++m_currentFaceIndex;  // <-- We need to add a new int m_currentFaceIndex attribute member to the class
        if (m_currentFaceIndex >= maxFaces) m_currentFaceIndex = 0;
        GameEngine::SpriteRenderComponent* render = GetEntity()->GetComponent<GameEngine::SpriteRenderComponent>();
        if (render)
        {
            render->SetTileIndex(sf::Vector2i(m_currentFaceIndex, 0));
        }
    }

    //By default the displacement is 0,0
    sf::Vector2f displacement{ 0.0f,0.0f };
    sf::Vector2f position = GetEntity()->GetPos();

    //The amount of speed that we will apply when input is received
    const float inputAmount = 300.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        if (Socket::isFish && playerSprite) {
            playerSprite->SetTexture(GameEngine::eTexture::Fish);
        }
        else {
            playerSprite->SetTexture(GameEngine::eTexture::Lettuce);
        }

        displacement.x -= inputAmount * dt;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        if (Socket::isFish) {
            playerSprite->SetTexture(GameEngine::eTexture::FishRight);
        }
        else {
            playerSprite->SetTexture(GameEngine::eTexture::LettuceRight);
        }

        displacement.x += inputAmount * dt; 
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
                if (position.x <= GetEntity()->GetSize().y / 2) {
                    sf::Vector2f fix{ GetEntity()->GetSize().y / 2 + 1, GetEntity()->GetPos().y };
                    GetEntity()->SetPos(fix);
                }
                displacement.x = 0.f;
                displacement.y = 0.f;
                velocity_x = 0.f;
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
                if (position.x > 1920 - GetEntity()->GetSize().y / 2) {
                    sf::Vector2f fix{ 1920 - GetEntity()->GetSize().y / 2, GetEntity()->GetPos().y };
                    GetEntity()->SetPos(fix);
                }
                displacement.x = 0.f;
                displacement.y = 0.f;
                velocity_x = 0.f;
        }
    }

    //For the time being just a simple intersection check that moves the entity out of all potential intersect boxes
    std::vector<CollidableComponent*>& collidables = CollisionManager::GetInstance()->GetCollidables();

    auto size = this->GetEntity()->GetSize();

    AABBRect intersection;
    AABBRect myBox = AABBRect(position, size);

    for (int a = 0; a < collidables.size(); ++a)
    {
        CollidableComponent* colComponent = collidables[a];
        if (colComponent->GetEntity() == this->GetEntity())
            continue;

        AABBRect collideBox = AABBRect(
            colComponent->GetEntity()->GetPos(), colComponent->GetEntity()->GetSize()); //colComponent->GetWorldAABB();
            // Collides A
            //          B
            // a.k.a. user has landed on a platform

        if (myBox.top + 1.9*collideBox.height >= collideBox.top &&
            myBox.top <= collideBox.top + collideBox.height &&
            myBox.left <= collideBox.left + myBox.width &&
            myBox.left + myBox.width >= collideBox.left) {

            velocity_y = 0.f;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                velocity_y = -6.f;
            }
        }
    }

    std::vector<GameEngine::Entity*> peppers = GameEngine::GameEngineMain::GetInstance()->m_gameBoard->peppers;

    for (GameEngine::Entity* pepper : peppers) {

        AABBRect chiliRect = AABBRect(pepper->GetPos(), pepper->GetSize());

        if (myBox.intersects(chiliRect)) {
            velocity_y += -8.f;
            srand((unsigned int)time(0));
            velocity_x = ((float)(rand() % 400 - 200) / 10.f);
            std::cout << velocity_x << "\n";
        }
    }

    // Update the entity position locally
    sf::Vector2f gravity{ velocity_x, velocity_y };

    displacement += gravity;

    sf::Vector2f newPos = GetEntity()->GetPos() + displacement;

    GetEntity()->SetPos(newPos);

    // Only send update to server when user has moved
    if (GetEntity()->GetPos() != position) {
        auto now = Clock::now();
        // Throttling update (1 req/10ms)
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastServerPositionUpdate).count() >= 10) {
            lastServerPositionUpdate = now;

            nlohmann::json j;
            j["x"] = GetEntity()->GetPos().x;
            j["y"] = GetEntity()->GetPos().y;
            Socket::io.socket()->emit("movePlayer", j.dump());
        }
    }
}

void PlayerMovementComponent::OnAddToWorld() {
    __super::OnAddToWorld();
    m_currentFaceIndex = 0;
    lastAnimation = 0.f;
}

PlayerMovementComponent::PlayerMovementComponent(){
}

PlayerMovementComponent::~PlayerMovementComponent() {}
