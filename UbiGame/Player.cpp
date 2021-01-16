#include "Player.h"
#include <string>

Player::Player(std::string playerId): Entity() { this->playerId = playerId; }
Player::~Player() {}