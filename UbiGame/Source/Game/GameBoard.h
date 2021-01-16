#pragma once
#include "GameEngine/EntitySystem/Entity.h"

#include <SFML/System/Vector2.hpp>
#include <vector>

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
	};

	class GameBoard
	{
	public:
		GameBoard();
		virtual ~GameBoard();

		void Update();		
		bool IsGameOver() { return false; }

	private:
		void CreatePlayer();          // <-- Added Function
		GameEngine::Entity* m_player; // <-- Added Member
		GameEngine::Entity* n_player; // <-- Added Member
		void AddBackground();
		GameEngine::Entity* bg; // <-- Added Member
		void AddObstacles();
		std::vector<GameEngine::Entity*> obstacles;
	};
}

