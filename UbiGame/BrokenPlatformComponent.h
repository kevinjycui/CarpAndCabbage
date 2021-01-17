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

		float velocity;

		virtual void Update() override;
		virtual void OnAddToWorld() override;
	};
}
