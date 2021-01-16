#include "GameEngine/GameEngineMain.h"
#include "../Socket.h"

int main()
{
	// Connect to Socket.IO server
	Socket::io.connect("http://127.0.0.1:3000");

	GameEngine::GameEngineMain* mainEngine = GameEngine::GameEngineMain::GetInstance();
	while (mainEngine->GetRenderWindow()->isOpen())
	{				
		mainEngine->Update();
	}

	delete mainEngine;
	return 0;
}