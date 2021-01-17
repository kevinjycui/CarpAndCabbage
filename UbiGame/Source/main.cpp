#include "GameEngine/GameEngineMain.h"
#include "../Socket.h"
#include "sio_socket.h"
#include <iostream>

// #define DEBUG

int main()
{
	// Connect to Socket.IO server
	#ifdef DEBUG
		Socket::io.connect("localhost:3000");
	#else
		Socket::io.connect("ws://carp-and-cabbage-server.herokuapp.com/");
	#endif

	Socket::io.socket()->emit("getPlayerId", {}, [&](sio::message::list const& msg) {
		std::string player_id = msg.at(0)->get_string();
		Socket::playerId = player_id;
	});

	GameEngine::GameEngineMain* mainEngine = GameEngine::GameEngineMain::GetInstance();
	while (mainEngine->GetRenderWindow()->isOpen())
	{				
		mainEngine->Update();
	}

	delete mainEngine;
	return 0;
}