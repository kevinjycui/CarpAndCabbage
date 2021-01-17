#include "BrokenPlatformComponent.h"
#include "GameEngine/GameEngineMain.h"
#include "GameEngine/EntitySystem/Components/CollidableComponent.h"
#include <iostream>

using namespace Game;

float acceleration = 0.098f;

BrokenPlatformComponent::BrokenPlatformComponent()
{

}

BrokenPlatformComponent::~BrokenPlatformComponent()

{

}

void BrokenPlatformComponent::Update()
{
	__super::Update();

	if (GetEntity()->GetPos().y >= 1100)
		velocity = -acceleration;

	velocity += acceleration;
	sf::Vector2f gravity{ 0.f, velocity };
	GetEntity()->SetPos(GetEntity()->GetPos() + gravity);

}

void BrokenPlatformComponent::OnAddToWorld()
{
	velocity = 0.f;
}
