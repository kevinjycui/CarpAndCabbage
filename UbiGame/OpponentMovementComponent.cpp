#include "OpponentMovementComponent.h"
#include <SFML/Window/Keyboard.hpp>   //<-- Add the keyboard include in order to get keyboard inputs

#include "GameEngine/GameEngineMain.h" //<-- Add this include to retrieve the delta time between frames
#include "json.hpp"
#include "../Socket.h"
#include <iostream>
#include <string>

using namespace Game;
using sio::socket;
using sio::message;

void OpponentMovementComponent::Update()
{
    __super::Update();
}

void OpponentMovementComponent::OnAddToWorld() {
    __super::OnAddToWorld();
}

OpponentMovementComponent::OpponentMovementComponent() {
    Socket::io.socket()->on("movePlayer", socket::event_listener_aux([&](std::string const& name, message::ptr const& data, bool is_ack, message::list& ack_resp) {
        std::cout << data->get_string();
        auto payload = nlohmann::json::parse(data->get_string());
        if (Socket::opponentId == payload["playerId"]) {
            sf::Vector2f pos{ payload["x"], payload["y"] };
            GetEntity()->SetPos(pos);
        }
    }));
}

OpponentMovementComponent::~OpponentMovementComponent() {}