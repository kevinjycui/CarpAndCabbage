#include "GameBoard.h"

#include "GameEngine/GameEngineMain.h"
#include "../../PlayerMovementComponent.h"
#include "../../OpponentMovementComponent.h"
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
#include "../ChiliArrowMovementComponent.h"
#include "../ChiliPepperMovementComponent.h"
#include "../Socket.h"
#include "sio_socket.h"
#include "json.hpp"
#include <iostream>

using sio::socket;
using sio::message;
using namespace Game;

static GameEngine::SoundComponent* soundCompon;
static int soundId;

GameBoard::GameBoard() {
	AddBackground();
	CreatePlayer();
	CreateOpponent();
	AddObstacles();

	Socket::io.socket()->on("chiliAttack", socket::event_listener_aux([&](std::string const& name, message::ptr const& data, bool is_ack, message::list& ack_resp) {
		auto payload = nlohmann::json::parse(data->get_string());
		float x = payload["x"];
		std::string activatedById = payload["activatedById"];

		// Creating the chili pepper
		GameEngine::Entity* chiliPepper = new GameEngine::Entity();
		GameEngine::GameEngineMain::GetInstance()->AddEntity(chiliPepper);

		if (activatedById == Socket::playerId) {
			chiliPepper->SetPos(sf::Vector2f(960.0f + x, 25.0f));
		}
		else {
			chiliPepper->SetPos(sf::Vector2f(0.0f + x, 25.0f));
		}

		chiliPepper->SetSize(sf::Vector2f(20.0f, 20.0f));
		chiliPepper->AddComponent<Game::ChiliPepperMovementComponent>();

		GameEngine::SpriteRenderComponent* chiliPepperSpriteRender = static_cast<GameEngine::SpriteRenderComponent*>(chiliPepper->AddComponent<GameEngine::SpriteRenderComponent>());

		chiliPepperSpriteRender->SetFillColor(sf::Color::Transparent);
		chiliPepperSpriteRender->SetTexture(GameEngine::eTexture::Knife);

		obstacles.push_back(chiliPepper);
	}));

	CreatePlatform();
}

Menu::Menu() {
	AddMenuBackground();
	AddButton();
	AddTextbox();
}


//if a button is pressed in the menu, call a function in the game engine main that destroys the menu object and calls the code that we removed
//destructor to get rid of the menu stuff
//gameenginemain::getinstance()->custom function

void Menu::AddMenuBackground() {
	bg = new GameEngine::Entity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(bg);

	bg->SetPos(sf::Vector2f(1920.0f / 2, 1080.0f / 2));
	bg->SetSize(sf::Vector2f(1920.0f, 1080.0f));

	GameEngine::SpriteRenderComponent* spriteRender = static_cast<GameEngine::SpriteRenderComponent*>
		(bg->AddComponent<GameEngine::SpriteRenderComponent>());

	spriteRender->SetFillColor(sf::Color::Transparent);
	spriteRender->SetTexture(GameEngine::eTexture::Background);
}

void Menu::AddButton() {
	btn = new GameEngine::Entity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(btn);
	btn->SetPos(sf::Vector2f(1920.0f / 2, 1080.0f / 2));
	btn->SetSize(sf::Vector2f(800.0f, 450.0f));

	GameEngine::SpriteRenderComponent* spriteRender = static_cast<GameEngine::SpriteRenderComponent*>
		(btn->AddComponent<GameEngine::SpriteRenderComponent>());

	spriteRender->SetFillColor(sf::Color::Transparent);
	spriteRender->SetTexture(GameEngine::eTexture::Title);

	soundCompon = static_cast<GameEngine::SoundComponent*>
		(btn->AddComponent<GameEngine::SoundComponent>());

	soundId = soundCompon->LoadSoundFromFile("Resources/audio/music.wav");
	soundCompon->PlaySound(soundId, true);
}

void Menu::AddTextbox() {

}

void Menu::Update() {
	//on click, call the function from gameenginemain
	//if button clicked
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
		Menu::~Menu();
		GameEngine::GameEngineMain::GetInstance()->StartGame(true);
	}
	// Start as opponent
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
		Menu::~Menu();
		GameEngine::GameEngineMain::GetInstance()->StartGame(false);
	}
}

Menu::~Menu() {

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

	// Creating user-moveable chili arrow
	GameEngine::Entity* chiliArrow = new GameEngine::Entity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(chiliArrow);

	if (Socket::isFish) {
		chiliArrow->SetPos(sf::Vector2f(960.0f, 0.0f));
	}
	else {
		chiliArrow->SetPos(sf::Vector2f(0.0f, 0.0f));
	}

	chiliArrow->SetSize(sf::Vector2f(20.0f, 20.0f));
	chiliArrow->AddComponent<Game::ChiliArrowMovementComponent>();

	GameEngine::SpriteRenderComponent* chiliArrowSpriteRender = static_cast<GameEngine::SpriteRenderComponent*>(chiliArrow->AddComponent<GameEngine::SpriteRenderComponent>());

	chiliArrowSpriteRender->SetFillColor(sf::Color::Transparent);
	chiliArrowSpriteRender->SetTexture(GameEngine::eTexture::Knife);

	obstacles.push_back(chiliArrow);
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

	// If player is the fish, opponent is the lettuce
	if (Socket::isFish) {
		opponent->SetPos(sf::Vector2f(2 * 640.0f, 150.0f));
	}
	else {
		opponent->SetPos(sf::Vector2f(640.0f, 150.0f));
	}

	opponent->SetSize(sf::Vector2f(128.0f, 128.0f));

	GameEngine::SpriteRenderComponent* spriteRender = static_cast<GameEngine::SpriteRenderComponent*>(opponent->AddComponent<GameEngine::SpriteRenderComponent>());
	spriteRender->SetFillColor(sf::Color::Transparent);

	if (Socket::isFish) {
		spriteRender->SetTexture(GameEngine::eTexture::Lettuce);
	}
	else {
		spriteRender->SetTexture(GameEngine::eTexture::Fish);
	}

	opponent->AddComponent<Game::OpponentMovementComponent>();
	opponent->AddComponent<GameEngine::CollidablePhysicsComponent>();

	// opponent->AddComponent<PawnPhysicsComponent>();

	this->opponent = opponent;
}

void GameBoard::CreatePlayer() {
	player = new Player(Socket::playerId);

	GameEngine::GameEngineMain::GetInstance()->AddEntity(player);

	if (Socket::isFish) {
		player->SetPos(sf::Vector2f(640.0f, 150.0f));
	}
	else {
		player->SetPos(sf::Vector2f(2 * 640.0f, 150.0f));
	}

	player->SetSize(sf::Vector2f(128.0f, 128.0f));

	GameEngine::SpriteRenderComponent* spriteRender = static_cast<GameEngine::SpriteRenderComponent*>(player->AddComponent<GameEngine::SpriteRenderComponent>());
	spriteRender->SetFillColor(sf::Color::Transparent);

	if (Socket::isFish) {
		spriteRender->SetTexture(GameEngine::eTexture::Fish);
	}
	else {
		spriteRender->SetTexture(GameEngine::eTexture::Lettuce);
	}

	player->AddComponent<Game::PlayerMovementComponent>();  // <-- Added the movement component to the player
	player->AddComponent<GameEngine::CollidablePhysicsComponent>();
	// player->AddComponent<PawnPhysicsComponent>();

	this->player = player;
}


GameBoard::~GameBoard()
{
}

int currPlatform = 0;
void GameBoard::Update()
{	
	//create global variable for how many platforms there are and give each one an index, top = 0
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		if (currPlatform > 0) {
			currPlatform--;
			//move selector to next platform
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		//move selector to previous platform
	}
}
