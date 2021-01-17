#include "BrokenPlatformComponent.h"
#include "GameEngine/GameEngineMain.h"
#include "GameEngine/EntitySystem/Components/CollidableComponent.h"

using namespace Game;

float velocity = 0.f;
float acceleration = 0.098f;

BrokenPlatformComponent::BrokenPlatformComponent()
	:m_lifeTime(3.f)
{

}

BrokenPlatformComponent::~BrokenPlatformComponent()

{

}

void BrokenPlatformComponent::Update()
{
	__super::Update();

	float dt = GameEngine::GameEngineMain::GetTimeDelta();

	m_lifeTime -= dt;

	if (m_lifeTime < 0.f) {
		//GameEngine::GameEngineMain::GetInstance()->RemoveEntity(GetEntity());
	}

	velocity += acceleration;
	sf::Vector2f gravity{ 0.f, velocity };
	GetEntity()->SetPos(GetEntity()->GetPos() + gravity);
}

void BrokenPlatformComponent::OnAddToWorld()
{

}
