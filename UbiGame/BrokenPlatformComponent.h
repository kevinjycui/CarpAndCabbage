#pragma once
#include "GameEngine/EntitySystem/Entity.h"
#include "GameEngine/EntitySystem/Components/CollidableComponent.h"

namespace Game
{
	class BrokenPlatformComponent : public GameEngine::CollidableComponent
	{
	public:
		BrokenPlatformComponent();
		~BrokenPlatformComponent();

		virtual void Update() override;
		virtual void OnAddToWorld() override;
		// change later
		void SetLifeTime(float lifetime) { m_lifeTime = lifetime; }

	private:
		float m_lifeTime;
	};
}
