#include "PawnPhysicsComponent.h"
#include "GameEngine/GameEngineMain.h"
#include "GameEngine/EntitySystem/Components/CollidableComponent.h"

using namespace Game;

void PawnPhysicsComponent::Update()
{
	float dt = GameEngine::GameEngineMain::GetTimeDelta();
	
	float currentY = GetEntity()->GetPos().y;
	float gForce = 700.f;

	if (dt > 0.f) {
		m_velocity.x = m_wantedVelocity.x;
		m_velocity.y += m_wantedVelocity.y;

		m_velocity.y += gForce * dt;

		sf::Vector2f deltaVelocity = dt * m_velocity;
		GetEntity()->SetPos(GetEntity()->GetPos() + deltaVelocity);
	}

	__super::Update();

	float deltaY = GetEntity()->GetPos().y - currentY;
	if (dt > 0.f) {
		m_velocity.y = deltaY / dt;
	}
}
