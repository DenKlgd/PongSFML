#include "SFML/Graphics.hpp"
#include "Ping-pong.h"
#include "GameStates.h"
#include "WindowInputHandlers.h"
#include "GUI.h"
#include "PongMenues.h"
#include "Network.h"
#include "Connection.h"

#ifdef _WIN32
	#include <Windows.h>
	#define ENTRY_POINT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
#else
	#define ENTRY_POINT main()
#endif


sf::RenderWindow mainRenderWindow;
sf::Vector2u windowSize(800, 480);
sf::Event event;
sf::Font font;
WindowInputHandlers::KeyboardInputHandler keyboardHandler;
GameState gameState;
Pong* pong = nullptr;
TCPnetwork::TCP_Base* network;
Points2D::Point2D scale {1.f, 1.f};

sf::UdpSocket udpSock;
sf::IpAddress remoteIP, remoteIPbuffer;
uint16_t port = 53000;
ConnectionStatus connectionStatus;

int ENTRY_POINT
{
	sf::Socket::Status status;
	char data[32]{'\0'};
	size_t received = 0;

	font.loadFromFile("Misc/Nautilus.otf");

	initMainMenu();
	initPauseMenu();
	initMultiplayerMenu();
	initConnectMenu();
	initWaitingForConnectionMenu();
	initOptionsMenu();

	gameState.setImmediately(EGameState::Menu);
	mainRenderWindow.create(sf::VideoMode(windowSize.x, windowSize.y), "Ping-pong", sf::Style::Close | sf::Style::Titlebar);

	while (mainRenderWindow.isOpen())
	{
		while (mainRenderWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				mainRenderWindow.close();

			if (event.type == sf::Event::LostFocus)
				keyboardHandler.resetPressedKeys();

			if (event.type == sf::Event::KeyPressed)
				keyboardHandler.pushKey(event.key.code);
			else if (event.type == sf::Event::KeyReleased)
				keyboardHandler.popKey(event.key.code);

			if (keyboardHandler.isPressed(sf::Keyboard::Escape))
			{
				EGameState currentGameState = gameState.getGameState();
				if (currentGameState == EGameState::InGame || currentGameState == EGameState::InMultiplayer)
					gameState.requestGameStateChange(EGameState::Pause);
			}

			switch (gameState.getGameState())
			{
			case EGameState::InGame:
				pong->UserInput();
				break;
			case EGameState::InMultiplayer:
				pong->UserInput();
				break;
			case EGameState::Menu:
				handleMainMenuInput(mainMenu.userInput(event));
				break;
			case EGameState::Pause:
				handlePauseMenuInput(pauseMenu.userInput(event));
				break;
			case EGameState::MultiplayerMenu:
				handleMultiplayerMenuInput(multiplayerMenu.userInput(event));
				break;
			case EGameState::ConnectMenu:
				handleConnectMenuInput(connectMenu.userInput(event));
				break;
			case EGameState::WaitingForOtherPlayer:
				handleWaitingForConnectionMenu(waitingForConnectionMenu.userInput(event));
				break;
			case EGameState::JoiningToHost:
				handleWaitingForConnectionMenu(waitingForConnectionMenu.userInput(event));
				break;
			case EGameState::SETTINGS:
				handleOptionsMenu(optionsMenu.userInput(event));
				break;
			default:
				break;
			}
		}

		if (gameState.getGameState() == EGameState::Pause)
		{
			pauseMenuNetworkUpdate();
		}

		switch (gameState.getGameState())
		{
		case EGameState::InGame:
			gameState.applyGameStateChange();
			pong->GameUpdate();
			mainRenderWindow.clear(sf::Color(20, 160, 132));
			pong->Render(scale);
			break;
		case EGameState::InMultiplayer:
			gameState.applyGameStateChange();
			if (dynamic_cast<PongClient*>(pong))
				pong->sendData();
			else
				pong->receiveData();
			pong->GameUpdate();
			if (dynamic_cast<PongHost*>(pong))
				pong->sendData();
			else
				pong->receiveData();
			mainRenderWindow.clear(sf::Color(20, 160, 132));
			pong->Render(scale);
			break;
		case EGameState::Menu:
			mainMenu.update();
			mainRenderWindow.clear(sf::Color(20, 160, 132));
			mainMenu.draw();
			break;
		case EGameState::Pause:
			pauseMenu.draw();
			break;
		case EGameState::MultiplayerMenu:
			mainRenderWindow.clear(sf::Color(20, 160, 132));
			multiplayerMenu.draw();
			break;
		case EGameState::ConnectMenu:
			connectMenu.update();
			mainRenderWindow.clear(sf::Color(20, 160, 132));
			connectMenu.draw();
			break;
		case EGameState::WaitingForOtherPlayer:
			if (connectionStatus == ConnectionStatus::WaitingForConnections)
			{
				received = 0;
				*data = '\0';
				status = udpSock.receive(data, sizeof(data), received, remoteIP, port);
				if (std::strcmp(data, "UDP_CLIENT_REQUEST") == 0)
					connectionStatus = ConnectionStatus::Replying;
			}

			if (connectionStatus == ConnectionStatus::Replying)
			{
				memcpy(data, "UDP_HOST_REPLY\0", strlen("UDP_HOST_REPLY") + 1);
				status = udpSock.send(data, strlen(data) + 1, remoteIP, port);
				if (status != sf::Socket::Status::Done)
					connectionStatus = ConnectionStatus::WaitingForConnections;
				else
					connectionStatus = ConnectionStatus::ConnectionEstablished;
			}

			if (connectionStatus == ConnectionStatus::ConnectionEstablished)
			{
				udpSock.unbind();
				remoteIP = sf::IpAddress::Any;			
				network = new TCPnetwork::Server;
				dynamic_cast<TCPnetwork::Server*>(network)->listenForConnections();
				pong = new PongHost;
				gameState.setImmediately(EGameState::InMultiplayer);
				pong->initGame();
			}
			waitingForConnectionMenu.update();
			mainRenderWindow.clear(sf::Color(20, 160, 132));
			waitingForConnectionMenu.draw();
			break;
		case EGameState::JoiningToHost:
			if (connectionStatus == ConnectionStatus::RequestingConnection)
			{
				memcpy(data, "UDP_CLIENT_REQUEST\0", strlen("UDP_CLIENT_REQUEST") + 1);
				status = udpSock.send(data, strlen(data) + 1, remoteIP, port);
				if (status == sf::Socket::Status::Done)
					connectionStatus = ConnectionStatus::WaitingForReply;
			}

			if (connectionStatus == ConnectionStatus::WaitingForReply)
			{
				received = 0;
				status = udpSock.receive(data, sizeof(data), received, remoteIP, port);
				if (status == sf::Socket::Status::Done)
				{
					if (strcmp(data, "UDP_HOST_REPLY") != 0)
						connectionStatus = ConnectionStatus::RequestingConnection;
					else
						connectionStatus = ConnectionStatus::ConnectionEstablished;
				}
				else
				{
					remoteIP = remoteIPbuffer;
					port = 53000;
					connectionStatus = ConnectionStatus::RequestingConnection;
				}
			}

			if (connectionStatus == ConnectionStatus::ConnectionEstablished)
			{
				network = new TCPnetwork::Client;
				dynamic_cast<TCPnetwork::Client*>(network)->connect(remoteIP.toString());
				pong = new PongClient;
				gameState.setImmediately(EGameState::InMultiplayer);
				pong->initGame();
			}
			waitingForConnectionMenu.update();
			mainRenderWindow.clear(sf::Color(20, 160, 132));
			waitingForConnectionMenu.draw();
			break;
		case EGameState::SETTINGS:
			mainRenderWindow.clear(sf::Color(20, 160, 132));
			optionsMenu.draw();
			break;
		default:
			break;
		}
		mainRenderWindow.display();
	}

	if (network != nullptr)
	{
		network->closeConnection();
		delete network;
	}
	if (pong != nullptr)
	{
		delete pong;
	}

	return 0;
}