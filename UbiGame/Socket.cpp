#include "Socket.h"
#include "sio_client.h"

sio::client Socket::io;
std::string Socket::playerId;
std::string Socket::opponentId;
bool Socket::isFish = false;
bool Socket::isGameOver = false;
bool Socket::isFullRoom = false;
bool Socket::firstInRoom = false;
bool Socket::isWaitingOnOtherPlayer = false;
bool Socket::isFishDead = false;
bool Socket::isCabbageDead = false;