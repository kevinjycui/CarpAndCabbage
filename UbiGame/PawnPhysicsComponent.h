#pragma once

#include "GameEngine/EntitySystem/Entity.h"
#include "GameEngine/EntitySystem/Components/CollidablePhysicsComponent.h"
#include <SFML/System/Vector2.hpp>

namespace Game
{
	class PawnPhysicsComponent : public GameEngine::CollidablePhysicsComponent
	{
	public:
		PawnPhysicsComponent();
		~PawnPhysicsComponent();

		virtual void OnAddToWorld() override;
		virtual void Update() override;
		
		void SetVelocity(sf::Vector2f vel) { m_wantedVelocity = vel; }

	private:
		sf::Vector2f m_wantedVelocity;
		sf::Vector2f m_velocity;
		sf::Vector2f m_lastPos;
	};
}