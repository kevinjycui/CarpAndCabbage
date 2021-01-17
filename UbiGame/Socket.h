#pragma once
#ifndef SOCKET_H
#define SOCKET_H

#include "sio_client.h"

class Socket {
public:
	static sio::client io;
	static std::string playerId;
	static std::string opponentId;
	static bool isFish;
	static bool isGameOver;
	static bool firstInRoom;
	static bool isFullRoom;
};

#endif