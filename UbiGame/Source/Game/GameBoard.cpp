#include "GameBoard.h"

#include "GameEngine/GameEngineMain.h"
#include "../../PlayerMovementComponent.h"
#include "../../KnifeMovementComponent.h"
#include "../../PlatformComponent.h"
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"
#include <GameEngine/EntitySystem/Components/SoundComponent.h>
#include "GameEngine/EntitySystem/Components/CollidableComponent.h"
#include "GameEngine/EntitySystem/Components/CollidablePhysicsComponent.h"
#include "../Player.h"
#include "../Socket.h"
#include <string>
#include "../../PawnPhysicsComponent.h"

using namespace Game;

static GameEngine::SoundComponent* soundCompon;
static int soundId;

GameBoard::GameBoard() {
	AddBackground();
	CreatePlayer();
	CreateOpponent();
	AddObstacles();
	CreatePlatform();
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
	const int num = 2;

	float x_coords[num]{ 640.f, 1280.f };
	float y_coords[num]{ 500.f, 800.f };

	for (int i = 0; i < num; i++) {

		GameEngine::Entity* knife = new GameEngine::Entity();

		GameEngine::GameEngineMain::GetInstance()->AddEntity(knife);

		knife->SetPos(sf::Vector2f(x_coords[i], y_coords[i]));
		knife->SetSize(sf::Vector2f(175.0f, 50.0f));

		knife->AddComponent<Game::KnifeMovementComponent>();
		knife->AddComponent<GameEngine::CollidableComponent>();

		GameEngine::SpriteRenderComponent* spriteRender = static_cast<GameEngine::SpriteRenderComponent*>(knife->AddComponent<GameEngine::SpriteRenderComponent>());

		spriteRender->SetFillColor(sf::Color::Transparent);
		spriteRender->SetTexture(GameEngine::eTexture::Knife);

		obstacles.push_back(knife);

	}

}

void GameBoard::CreatePlatform(){

	const int num = 5;
	
	float x_coords[num]{ 320.f, 640.f, 960.f, 1280.f, 1600.f  };
	float y_coords[num]{ 720.f, 550.f, 720.f, 550.f, 720.f };

	for (int i = 0; i < num; i++) {

		GameEngine::Entity* platform = new GameEngine::Entity();

		GameEngine::GameEngineMain::GetInstance()->AddEntity(platform);

		platform->SetPos(sf::Vector2f(x_coords[i], y_coords[i]));
		platform->SetSize(sf::Vector2f(175.0f, 50.0f));

		GameEngine::SpriteRenderComponent* spriteRender = static_cast<GameEngine::SpriteRenderComponent*>(platform->AddComponent<GameEngine::SpriteRenderComponent>());

		spriteRender->SetFillColor(sf::Color::Transparent);
		spriteRender->SetTexture(GameEngine::eTexture::Bread);

		platform->AddComponent<GameEngine::CollidableComponent>();
		platform->AddComponent<PlatformComponent>();

	}
}

void GameBoard::CreateOpponent() {
	opponent = new Player(Socket::opponentId);

	GameEngine::GameEngineMain::GetInstance()->AddEntity(opponent);

	opponent->SetPos(sf::Vector2f(2 * 640.0f, 150.0f));
	opponent->SetSize(sf::Vector2f(128.0f, 128.0f));

	GameEngine::SpriteRenderComponent* spriteRender = static_cast<GameEngine::SpriteRenderComponent*>(opponent->AddComponent<GameEngine::SpriteRenderComponent>());

	spriteRender->SetFillColor(sf::Color::Transparent);
	spriteRender->SetTexture(GameEngine::eTexture::Lettuce);

	opponent->AddComponent<Game::PlayerMovementComponent>();
	opponent->AddComponent<GameEngine::CollidablePhysicsComponent>();

	// opponent->AddComponent<PawnPhysicsComponent>();

	this->opponent = opponent;
}

void GameBoard::CreatePlayer() {
	player = new Player(Socket::playerId);

	GameEngine::GameEngineMain::GetInstance()->AddEntity(player);

	player->SetPos(sf::Vector2f(640.0f, 150.0f));
	player->SetSize(sf::Vector2f(128.0f, 128.0f));

	GameEngine::SpriteRenderComponent* spriteRender = static_cast<GameEngine::SpriteRenderComponent*>(player->AddComponent<GameEngine::SpriteRenderComponent>());

	spriteRender->SetFillColor(sf::Color::Transparent);
	spriteRender->SetTexture(GameEngine::eTexture::Fish);


	player->AddComponent<Game::PlayerMovementComponent>();  // <-- Added the movement component to the player
	player->AddComponent<GameEngine::CollidablePhysicsComponent>();
	// player->AddComponent<PawnPhysicsComponent>();

	this->player = player;
}


GameBoard::~GameBoard()
{
}


void GameBoard::Update()
{	
	
}
