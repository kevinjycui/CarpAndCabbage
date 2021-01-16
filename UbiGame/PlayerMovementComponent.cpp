#include "PlayerMovementComponent.h"
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
#include "KnifeMovementComponent.h"

using namespace GameEngine;
using namespace Game;
using sio::socket;
using sio::message;

static float velocity_y = 0.f;
static float acceleration_y = 0.098f;

void PlayerMovementComponent::Update()
{
    __super::Update();

    velocity_y += acceleration_y;

    GameEngine::SpriteRenderComponent* playerSprite = GetEntity()->GetComponent<GameEngine::SpriteRenderComponent>();

    //Grabs how much time has passed since last frame
    const float dt = GameEngine::GameEngineMain::GetTimeDelta();

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
        }
    }

    //For the time being just a simple intersection check that moves the entity out of all potential intersect boxes
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

    // Update the entity position locally
    sf::Vector2f gravity{ 0.0f, velocity_y };

    displacement += gravity;

    sf::Vector2f newPos = GetEntity()->GetPos() + displacement;

    GetEntity()->SetPos(newPos);

    // Only send update to server when user has moved
    if (GetEntity()->GetPos() != position) {
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
}

PlayerMovementComponent::~PlayerMovementComponent() {}
