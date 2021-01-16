#define _WEBSOCKETPP_CPP11_FUNCTIONAL_
#include "GameEngine/GameEngineMain.h"
#include "sio_client.h"

sio::client h;

int main()
{
	GameEngine::GameEngineMain* mainEngine = GameEngine::GameEngineMain::GetInstance();
	while (mainEngine->GetRenderWindow()->isOpen())
	{				
		mainEngine->Update();
	}

	delete mainEngine;
	return 0;
}