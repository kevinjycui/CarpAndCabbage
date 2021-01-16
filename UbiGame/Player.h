#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "sio_client.h"
#include "GameEngine/EntitySystem/Entity.h"

#include <string>

class Player : public GameEngine::Entity {
public:
	Player(std::string playerId);
	~Player();

	std::string playerId;
};
#endif