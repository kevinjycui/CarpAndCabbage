#pragma once
#include "GameEngine/EntitySystem/Entity.h"
#include "GameEngine/EntitySystem/Components/TextRenderComponent.h"
#include <SFML/System/Vector2.hpp>
#include <vector>
#include "../Player.h"
#include "../Socket.h"

namespace Game
{
	//Used for storing and controlling all game related entities, as well as providing an entry point for the "game" side of application	
	class PlayerEntity;

	class Menu
	{
	public:
		Menu();
		virtual ~Menu();
		void Update();

	private:
		void AddMenuBackground();
		GameEngine::Entity* bg;
		void AddButton();
		GameEngine::Entity* btn;
		void AddTextbox();
		GameEngine::TextRenderComponent* textbox;
	};

	class GameOver
	{
	public:
		GameOver();
		virtual ~GameOver();
		void Update();
	private:
		void AddGOBackground();
		GameEngine::Entity* bg;
		//void AddMenuBackground();
		//GameEngine::Entity* bg;
		//void AddButton();
		//GameEngine::Entity* btn;
		//void AddTextbox();
		//GameEngine::TextRenderComponent* textbox;
	};

	class GameBoard
	{
	public:
		GameBoard();
		virtual ~GameBoard();

		void Update();
		bool IsGameOver() { return Socket::isGameOver; }

		void CreatePlayer();   // <-- Added Function
		void CreateOpponent();
		Player* player; // <-- Added Member
		Player* opponent; // <-- Added Member

		void AddBackground();
		GameEngine::Entity* bg; // <-- Added Member
		void AddObstacles();
		void CreatePlatform();
		void CreatePepper();
		void SpawnPepper(sf::Vector2f pos);
		std::vector<GameEngine::Entity*> obstacles;
		std::vector<GameEngine::Entity*> fishPlatforms;
		std::vector<GameEngine::Entity*> cabbagePlatforms;
		void CreateCuts();
		GameEngine::Entity* cut;
	};
}