#pragma once
#include "GameEngine/EntitySystem/Entity.h"
#include "GameEngine/EntitySystem/Components/CollidableComponent.h"

namespace Game
{
	class PlatformComponent : public GameEngine::CollidableComponent
	{
	public:
		PlatformComponent();
		~PlatformComponent();

		virtual void Update() override;

	};
}