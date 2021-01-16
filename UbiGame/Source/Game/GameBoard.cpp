#include "GameBoard.h"

#include "GameEngine/GameEngineMain.h"
#include "../../PlayerMovementComponent.h"
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"
#include <GameEngine/EntitySystem/Components/SoundComponent.h>

using namespace Game;

static GameEngine::SoundComponent* soundCompon;
static int soundId;

GameBoard::GameBoard() {
	CreatePlayer();
}

void GameBoard::CreatePlayer()
{
	m_player = new GameEngine::Entity();
	n_player = new GameEngine::Entity();

	soundCompon = static_cast<GameEngine::SoundComponent*>
		(m_player->AddComponent<GameEngine::SoundComponent>());

	soundId = soundCompon->LoadSoundFromFile("Resources/audio/music.wav");
	soundCompon->PlaySound(soundId, true);

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
}

GameBoard::~GameBoard()
{
}


void GameBoard::Update()
{	
	
}