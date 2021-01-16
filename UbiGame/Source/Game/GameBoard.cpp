#include "GameBoard.h"

#include "GameEngine/GameEngineMain.h"
#include "../../PlayerMovementComponent.h"

using namespace Game;

GameBoard::GameBoard()
{
	CreatePlayer();
}

void GameBoard::CreatePlayer()
{
	m_player = new GameEngine::Entity();
	n_player = new GameEngine::Entity();

	GameEngine::GameEngineMain::GetInstance()->AddEntity(m_player);
	GameEngine::GameEngineMain::GetInstance()->AddEntity(n_player);

	m_player->SetPos(sf::Vector2f(640.0f, 50.0f));
	m_player->SetSize(sf::Vector2f(50.0f, 50.0f));

	n_player->SetPos(sf::Vector2f(2 * 640.0f, 50.0f));
	n_player->SetSize(sf::Vector2f(50.0f, 50.0f));

	//Render
	GameEngine::RenderComponent* render1 = m_player->AddComponent<GameEngine::RenderComponent>();
	GameEngine::RenderComponent* render2 = n_player->AddComponent<GameEngine::RenderComponent>();

	render1->SetFillColor(sf::Color::Red);
	render2->SetFillColor(sf::Color::Blue);

	//Movement
	m_player->AddComponent<Game::PlayerMovementComponent>();  // <-- Added the movement component to the player
	n_player->AddComponent<Game::PlayerMovementComponent>();  // <-- Added the movement component to the player
}

GameBoard::~GameBoard()
{
}


void GameBoard::Update()
{	
	
}