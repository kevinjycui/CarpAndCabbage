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
#include "GameEngine/EntitySystem/Components/TextRenderComponent.h"
#include "../../BrokenPlatformComponent.h"

using sio::socket;
using sio::message;
using namespace Game;

static GameEngine::SoundComponent* soundCompon;
static int soundId;

void GameBoard::SpawnPepper(sf::Vector2f position) {

	// Creating the chili pepper
	GameEngine::Entity* chiliPepper = new GameEngine::Entity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(chiliPepper);

	chiliPepper->SetPos(position);
	chiliPepper->SetSize(sf::Vector2f(50.0f, 50.0f));
	chiliPepper->AddComponent<Game::ChiliPepperMovementComponent>();

	GameEngine::SpriteRenderComponent* chiliPepperSpriteRender = static_cast<GameEngine::SpriteRenderComponent*>(chiliPepper->AddComponent<GameEngine::SpriteRenderComponent>());

	chiliPepperSpriteRender->SetFillColor(sf::Color::Transparent);
	chiliPepperSpriteRender->SetTexture(GameEngine::eTexture::ChiliPepper);

	peppers.push_back(chiliPepper);
}

GameBoard::GameBoard() {
	AddBackground();
	CreatePlayer();
	CreateOpponent();
	AddObstacles();

	Socket::io.socket()->on("chiliAttack", socket::event_listener_aux([&](std::string const& name, message::ptr const& data, bool is_ack, message::list& ack_resp) {
		auto payload = nlohmann::json::parse(data->get_string());
		float x = payload["x"];
		float y = payload["y"];
		SpawnPepper(sf::Vector2f { x, y });
	}));

	CreatePlatform();
	CreateCuts();
	CreatePepper();
}

void GameBoard::CreatePepper() {
	// Creating user-moveable chili arrow
	GameEngine::Entity* chiliArrow = new GameEngine::Entity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(chiliArrow);

	if (Socket::isFish) {
		chiliArrow->SetPos(sf::Vector2f(1440.f, 50.f));
	}
	else {
		chiliArrow->SetPos(sf::Vector2f(480.f, 50.f));
	}

	chiliArrow->SetSize(sf::Vector2f(50.0f, 50.0f));
	chiliArrow->AddComponent<Game::ChiliArrowMovementComponent>();

	GameEngine::SpriteRenderComponent* chiliArrowSpriteRender = static_cast<GameEngine::SpriteRenderComponent*>(chiliArrow->AddComponent<GameEngine::SpriteRenderComponent>());

	chiliArrowSpriteRender->SetFillColor(sf::Color::Transparent);
	chiliArrowSpriteRender->SetTexture(GameEngine::eTexture::ChiliPepper);

	obstacles.push_back(chiliArrow);
}

Menu::Menu() {
	AddMenuBackground();

	// Room was full
	Socket::io.socket()->on("fullRoom", socket::event_listener_aux([&](std::string const& name, message::ptr const& data, bool is_ack, message::list& ack_resp) {
		Socket::isFullRoom = true;
		Socket::isWaitingOnOtherPlayer = false;
	}));

	// You started a room
	Socket::io.socket()->on("firstInRoom", socket::event_listener_aux([&](std::string const& name, message::ptr const& data, bool is_ack, message::list& ack_resp) {
		Socket::firstInRoom = true;
	}));

	// A player joined your room
	Socket::io.socket()->on("newPlayer", socket::event_listener_aux([&](std::string const& name, message::ptr const& data, bool is_ack, message::list& ack_resp) {
		std::string opponentId = data->get_string();
		Socket::opponentId = opponentId;

		// Start the game
		Menu::~Menu();
		GameEngine::GameEngineMain::GetInstance()->StartGame(true);
	}));

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
	textbox = new GameEngine::TextRenderComponent();
	// GameEngine::GameEngineMain::GetInstance()->AddEntity(dynamic_cast<GameEngine::Entity*>(textbox));
	// textbox->SetString("Hi");
}

void Menu::Update() {
	// Debug key for 1-player
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
		// Start the game
		Menu::~Menu();
		GameEngine::GameEngineMain::GetInstance()->StartGame(true);
	}

	//on click, call the function from gameenginemain
	//if button clicked

	// Start as fish
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
		if (!Socket::isWaitingOnOtherPlayer) {
			Socket::isWaitingOnOtherPlayer = true;
			Socket::isFish = true;
			// Getting socket.io connection
			Socket::io.socket()->emit("joinRoom", std::string("roomId"), [&](sio::message::list const& msg) {
				std::string result_str = msg.at(0)->get_string();
				// Non-empty string is opponentId
				if (!result_str.empty()) {
					Socket::opponentId = result_str;

					// Start the game
					Menu::~Menu();
					GameEngine::GameEngineMain::GetInstance()->StartGame(true);
				}
			});
		}
	}

	// Start as cabbage
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
		if (!Socket::isWaitingOnOtherPlayer) {
			Socket::isWaitingOnOtherPlayer = true;
			Socket::isFish = false;
			// Getting socket.io connection
			Socket::io.socket()->emit("joinRoom", std::string("roomId"), [&](sio::message::list const& msg) {
				std::string result_str = msg.at(0)->get_string();
				// Non-empty string is opponentId
				if (!result_str.empty()) {
					Socket::opponentId = result_str;

					// Start the game
					Menu::~Menu();
					GameEngine::GameEngineMain::GetInstance()->StartGame(false);
				}
			});
		}
	}
}

Menu::~Menu() {

}


void GameOver::Update() {
	//on click, call the function from gameenginemain
	//if button clicked
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
		GameOver::~GameOver();
		GameEngine::GameEngineMain::GetInstance()->EndGame();
		std::cout << "game over";
	}
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

}

std::vector<sf::Vector2f> fishPlatformCoords = { sf::Vector2f(320.f, 380.f) , sf::Vector2f(640.f, 550.f), sf::Vector2f(320.f, 720.f) };
std::vector<sf::Vector2f> cabbagePlatformCoords = { sf::Vector2f(1600.f, 380.f), sf::Vector2f(1280.f, 550.f), sf::Vector2f(1600.f, 720.f)};

GameEngine::Entity* brokenFish = new GameEngine::Entity();
GameEngine::Entity* brokenCabbage = new GameEngine::Entity();

void GameBoard::CreatePlatform(){
	//float x_coords[num]{ 320.f, 640.f, 960.f, 1280.f, 1600.f  };
	//float y_coords[num]{ 720.f, 550.f, 720.f, 550.f, 720.f };

	for (int i = 0; i < fishPlatformCoords.size(); i++) {

		GameEngine::Entity* platform = new GameEngine::Entity();

		GameEngine::GameEngineMain::GetInstance()->AddEntity(platform);

		platform->SetPos(fishPlatformCoords.at(i));
		platform->SetSize(sf::Vector2f(175.0f, 50.0f));

		GameEngine::SpriteRenderComponent* spriteRender = static_cast<GameEngine::SpriteRenderComponent*>(platform->AddComponent<GameEngine::SpriteRenderComponent>());

		spriteRender->SetFillColor(sf::Color::Transparent);
		spriteRender->SetTexture(GameEngine::eTexture::Bread);

		platform->AddComponent<GameEngine::CollidableComponent>();
		platform->AddComponent<PlatformComponent>();

		fishPlatforms.push_back(platform);
	}
	for (int i = 0; i < cabbagePlatformCoords.size(); i++) {

		GameEngine::Entity* platform = new GameEngine::Entity();

		GameEngine::GameEngineMain::GetInstance()->AddEntity(platform);

		platform->SetPos(cabbagePlatformCoords.at(i));
		platform->SetSize(sf::Vector2f(175.0f, 50.0f));

		GameEngine::SpriteRenderComponent* spriteRender = static_cast<GameEngine::SpriteRenderComponent*>(platform->AddComponent<GameEngine::SpriteRenderComponent>());

		spriteRender->SetFillColor(sf::Color::Transparent);
		spriteRender->SetTexture(GameEngine::eTexture::Bread);

		platform->AddComponent<GameEngine::CollidableComponent>();
		platform->AddComponent<PlatformComponent>();

		cabbagePlatforms.push_back(platform);
	}

	GameEngine::GameEngineMain::GetInstance()->AddEntity(brokenFish);

	brokenFish->SetPos(sf::Vector2f(1960.f, 2000.f));
	brokenFish->SetSize(sf::Vector2f(175.0f, 50.0f));

	GameEngine::SpriteRenderComponent* brokenFishSpriteRender = static_cast<GameEngine::SpriteRenderComponent*>(brokenFish->AddComponent<GameEngine::SpriteRenderComponent>());

	brokenFishSpriteRender->SetFillColor(sf::Color::Transparent);
	brokenFishSpriteRender->SetTexture(GameEngine::eTexture::BrokenBread);

	brokenFish->AddComponent<Game::BrokenPlatformComponent>();

	GameEngine::GameEngineMain::GetInstance()->AddEntity(brokenCabbage);

	brokenCabbage->SetPos(sf::Vector2f(1960.f, 2000.f));
	brokenCabbage->SetSize(sf::Vector2f(175.0f, 50.0f));

	GameEngine::SpriteRenderComponent* brokenCabbageSpriteRender = static_cast<GameEngine::SpriteRenderComponent*>(brokenCabbage->AddComponent<GameEngine::SpriteRenderComponent>());

	brokenCabbageSpriteRender->SetFillColor(sf::Color::Transparent);
	brokenCabbageSpriteRender->SetTexture(GameEngine::eTexture::BrokenBread);

	brokenCabbage->AddComponent<Game::BrokenPlatformComponent>();
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

int currPlatform = 1;
GameEngine::Entity* cut;
sf::Vector2f newPos{ 0.f, 0.f };
bool cutMade = false;
bool up_pressed = false;
bool down_pressed = false;

float time_cut = 0.f;

bool comparator(const GameEngine::Entity* lhs, const GameEngine::Entity* rhs) {
	return lhs->GetPos().y < rhs->GetPos().y;
}

void GameBoard::BreakPlayerPlatform(sf::Vector2f pos) {
	std::vector<GameEngine::Entity*>* opponentPlatforms;
	if (Socket::isFish)
		opponentPlatforms = &cabbagePlatforms;
	else
		opponentPlatforms = &fishPlatforms;

	cutMade = true;

	GameEngine::Entity* platform = opponentPlatforms->at(currPlatform);

	newPos = platform->GetPos();

	if (Socket::isFish)
		brokenFish->SetPos(newPos);
	else
		brokenCabbage->SetPos(newPos);

	cabbagePlatforms.erase(cabbagePlatforms.begin() + currPlatform);
	currPlatform = 1;

	GameEngine::GameEngineMain::GetInstance()->RemoveEntity(platform);
	GameEngine::GameEngineMain::GetInstance()->RemoveEntity(cut);

	cabbagePlatformCoords;
}

void GameBoard::BreakOpponentPlatform(sf::Vector2f pos) {
	std::vector<GameEngine::Entity*>* opponentPlatforms;
	if (Socket::isFish)
		opponentPlatforms = &cabbagePlatforms;
	else
		opponentPlatforms = &fishPlatforms;

	for (int i = 0; i < opponentPlatforms->size(); ++i) {
		if (opponentPlatforms->at(i)->GetPos() == pos) {

		}
	}
}

void GameBoard::Update()
{
	std::vector<GameEngine::Entity*>* opponentPlatforms;
	if (Socket::isFish)
		opponentPlatforms = &cabbagePlatforms;
	else
		opponentPlatforms = &fishPlatforms;

	if (cutMade) {
		time_cut += GameEngine::GameEngineMain::GetTimeDelta();
		if (time_cut > 5.f) {
			time_cut = 0;
			GameEngine::Entity* newPlatform = new GameEngine::Entity();

			newPlatform->SetPos(newPos);
			newPlatform->SetSize(sf::Vector2f(175.0f, 50.0f));

			GameEngine::SpriteRenderComponent* newSpriteRender = static_cast<GameEngine::SpriteRenderComponent*>(newPlatform->AddComponent<GameEngine::SpriteRenderComponent>());

			newSpriteRender->SetFillColor(sf::Color::Transparent);
			newSpriteRender->SetTexture(GameEngine::eTexture::Bread);

			newPlatform->AddComponent<GameEngine::CollidableComponent>();
			newPlatform->AddComponent<PlatformComponent>();

			GameEngine::GameEngineMain::GetInstance()->AddEntity(newPlatform);

			opponentPlatforms->push_back(newPlatform);

			std::sort(opponentPlatforms->begin(), opponentPlatforms->end(), comparator);

			cutMade = false;
			CreateCuts();
		}
	}

	//create global variable for how many platforms there are and give each one an index, top = 0
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !cutMade) {
		up_pressed = true;
	} else if (up_pressed) {
		up_pressed = false;
		if (currPlatform > 0) {
			currPlatform--;
			cut->SetPos(opponentPlatforms->at(currPlatform)->GetPos());//cut.setpos
			//move selector to next platform
		}
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !cutMade) {
		down_pressed = true;
	} else if (down_pressed) {
		down_pressed = false;
		if (currPlatform < opponentPlatforms->size() - 1) {
			currPlatform++;
			cut->SetPos(opponentPlatforms->at(currPlatform)->GetPos());//cut.setpos
		}
	}

	//move selector to previous platform
	if (!cutMade && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		cutMade = true;

		GameEngine::Entity* platform = opponentPlatforms->at(currPlatform);

		newPos = platform->GetPos();

		if (Socket::isFish)
			brokenFish->SetPos(newPos);
		else
			brokenCabbage->SetPos(newPos);

		cabbagePlatforms.erase(cabbagePlatforms.begin() + currPlatform);
		currPlatform = 1;

		GameEngine::GameEngineMain::GetInstance()->RemoveEntity(platform);
		GameEngine::GameEngineMain::GetInstance()->RemoveEntity(cut);
	}
	else {
		down_pressed = false;
		up_pressed = false;
	}

	if (Socket::isGameOver) {
		GameEngine::GameEngineMain::GetInstance()->EndGame();
	}
}

void GameBoard::CreateCuts() {
	cut = new GameEngine::Entity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(cut);
	cut->SetSize(sf::Vector2f(8.0f, 65.0f));
	cut->SetPos(sf::Vector2f(1960.f, 2000.f));
	GameEngine::SpriteRenderComponent* spriteRender = static_cast<GameEngine::SpriteRenderComponent*>(cut->AddComponent<GameEngine::SpriteRenderComponent>());
	spriteRender->SetFillColor(sf::Color::Transparent);
	spriteRender->SetTexture(GameEngine::eTexture::DottedLine);
}

GameOver::GameOver() {
	AddGOBackground();
}

GameOver::~GameOver() {

}

void GameOver::AddGOBackground(){
	bg = new GameEngine::Entity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(bg);

	bg->SetPos(sf::Vector2f(1920.0f / 2, 1080.0f / 2));
	bg->SetSize(sf::Vector2f(1920.0f, 1080.0f));

	GameEngine::SpriteRenderComponent* spriteRender = static_cast<GameEngine::SpriteRenderComponent*>
		(bg->AddComponent<GameEngine::SpriteRenderComponent>());

	spriteRender->SetFillColor(sf::Color::Transparent);
	spriteRender->SetTexture(GameEngine::eTexture::GameEnd);
}