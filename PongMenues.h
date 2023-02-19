#pragma once

#include "SFML/Graphics.hpp"
#include "Ping-Pong.h"
#include "GameStates.h"
#include "WindowInputHandlers.h"
#include "GUI.h"
#include "Network.h"
#include "Connection.h"

enum class MainMenuElements : unsigned char
{
	PONG_TITLE,
	PVP,
	MULTIPLAYER,
	EXIT,
};

enum class PauseMenuElements : unsigned char
{
	RESUME,
	RETURN_TO_MENU,
};

enum class MultiplayerMenuElements : unsigned char
{
	HOST,
	CONNECT,
	RETURN_TO_MENU,
};

enum class ConnectionMenuElements : unsigned char
{
	IP_TEXTBOX,
	CONNECT,
	RETURN_TO_MENU,
};

enum class WaitingForConnectionMenu : unsigned char
{
	Title,
	LoadingCircle,
	Cancel,
};

extern sf::RenderWindow mainRenderWindow;
extern sf::Font font;
extern sf::Vector2u windowSize;
extern sf::Event event;
extern GameState gameState;
extern WindowInputHandlers::KeyboardInputHandler keyboardHandler;
extern GUI::Menu mainMenu;
extern GUI::Menu pauseMenu;
extern GUI::Menu multiplayerMenu;
extern GUI::Menu connectMenu;
extern GUI::Menu waitingForConnectionMenu;
extern Pong* pong;
extern TCPnetwork::TCP_Base* network;
extern sf::UdpSocket udpSock;
extern sf::IpAddress remoteIP, remoteIPbuffer;
extern uint16_t port;
extern ConnectionStatus connectionStatus;

void initMainMenu();
void initPauseMenu();
void initConnectMenu();
void initMultiplayerMenu();
void initWaitingForConnectionMenu();
void handleMainMenuInput(GUI::ElementID elementID);
void handlePauseMenuInput(GUI::ElementID elementID);
void handleConnectMenuInput(GUI::ElementID elementID);
void handleMultiplayerMenuInput(GUI::ElementID elementID);
void handleWaitingForConnectionMenu(GUI::ElementID elementID);
void pauseMenuNetworkUpdate();