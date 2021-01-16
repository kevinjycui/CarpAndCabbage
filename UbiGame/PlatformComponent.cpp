#include "PlatformComponent.h"
#include "GameEngine/GameEngineMain.h"
#include "GameEngine/EntitySystem/Components/CollidableComponent.h"

using namespace Game;

PlatformComponent::PlatformComponent()
	:m_lifeTime(100.f)
{

}

PlatformComponent::~PlatformComponent()

{

}

void PlatformComponent::Update()
{
	__super::Update();

	float dt = GameEngine::GameEngineMain::GetTimeDelta();

	m_lifeTime -= dt;

	if (m_lifeTime < 0.f) {
		GameEngine::GameEngineMain::GetInstance()->RemoveEntity(GetEntity());
	}
}
