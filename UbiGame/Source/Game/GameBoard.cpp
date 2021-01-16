#include "GameBoard.h"

#include "GameEngine/GameEngineMain.h"
#include "../../PlayerMovementComponent.h"
#include "../../KnifeMovementComponent.h"
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"
#include <GameEngine/EntitySystem/Components/SoundComponent.h>
#include "GameEngine/EntitySystem/Components/CollidableComponent.h"
#include "GameEngine/EntitySystem/Components/CollidablePhysicsComponent.h"

using namespace Game;

static GameEngine::SoundComponent* soundCompon;
static int soundId;

GameBoard::GameBoard() {
	AddBackground();
	CreatePlayer();
	AddObstacles();
}

void GameBoard::AddBackground()
{
	bg = new GameEngine::Entity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(bg);

	bg->SetPos(sf::Vector2f(1920.0f / 2, 1080.0f / 2));
	bg->SetSize(sf::Vector2f(1920.0f, 1080.0f));

	GameEngine::SpriteRenderComponent* spriteRender = static_cast<GameEngine::SpriteRenderComponent*>
		(bg->AddComponent<GameEngine::SpriteRenderComponent>());

	spriteRender->SetFillColor(sf::Color::Transparent);
	spriteRender->SetTexture(GameEngine::eTexture::Background);

	soundCompon = static_cast<GameEngine::SoundComponent*>
		(bg->AddComponent<GameEngine::SoundComponent>());

	soundId = soundCompon->LoadSoundFromFile("Resources/audio/music.wav");
	soundCompon->PlaySound(soundId, true);
}

void GameBoard::AddObstacles()
{	
	GameEngine::Entity* knife = new GameEngine::Entity();

	GameEngine::GameEngineMain::GetInstance()->AddEntity(knife);

	knife->SetPos(sf::Vector2f(640.0f, 550.0f));
	knife->SetSize(sf::Vector2f(175.0f, 50.0f));

	knife->AddComponent<Game::KnifeMovementComponent>();

	GameEngine::SpriteRenderComponent* spriteRender = static_cast<GameEngine::SpriteRenderComponent*>(knife->AddComponent<GameEngine::SpriteRenderComponent>());

	spriteRender->SetFillColor(sf::Color::Transparent);
	spriteRender->SetTexture(GameEngine::eTexture::Knife);

	obstacles.push_back(knife);

	GameEngine::Entity* platform = new GameEngine::Entity();

	GameEngine::GameEngineMain::GetInstance()->AddEntity(platform);

	platform->SetPos(sf::Vector2f(640.0f, 550.0f));
	platform->SetSize(sf::Vector2f(175.0f, 50.0f));

	GameEngine::RenderComponent* render = platform->AddComponent<GameEngine::RenderComponent>(); // <-- Capturing the new component

	render->SetFillColor(sf::Color::Red); // <-- Change the fill color to Red

	platform->AddComponent<GameEngine::CollidableComponent>();
}

void GameBoard::CreatePlayer()
{
	m_player = new GameEngine::Entity();
	n_player = new GameEngine::Entity();

	GameEngine::GameEngineMain::GetInstance()->AddEntity(m_player);
	GameEngine::GameEngineMain::GetInstance()->AddEntity(n_player);

	m_player->SetPos(sf::Vector2f(640.0f, 150.0f));
	m_player->SetSize(sf::Vector2f(128.0f, 128.0f));

	n_player->SetPos(sf::Vector2f(2 * 640.0f, 150.0f));
	n_player->SetSize(sf::Vector2f(128.0f, 128.0f));

	//Render
	GameEngine::SpriteRenderComponent* spriteRender1 = static_cast<GameEngine::SpriteRenderComponent*>(m_player->AddComponent<GameEngine::SpriteRenderComponent>());
	GameEngine::SpriteRenderComponent* spriteRender2 = static_cast<GameEngine::SpriteRenderComponent*>(n_player->AddComponent<GameEngine::SpriteRenderComponent>());

	spriteRender1->SetFillColor(sf::Color::Transparent);
	spriteRender1->SetTexture(GameEngine::eTexture::Fish);
	
	spriteRender2->SetFillColor(sf::Color::Transparent);
	spriteRender2->SetTexture(GameEngine::eTexture::Lettuce);

	//Movement
	m_player->AddComponent<Game::PlayerMovementComponent>();  // <-- Added the movement component to the player
	n_player->AddComponent<Game::PlayerMovementComponent>();  // <-- Added the movement component to the player

	m_player->AddComponent < GameEngine::CollidablePhysicsComponent>();
	n_player->AddComponent < GameEngine::CollidablePhysicsComponent>();
}

GameBoard::~GameBoard()
{
}


void GameBoard::Update()
{	
	
}