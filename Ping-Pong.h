#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include "SFML/Graphics.hpp"
#include "Point2D.h"
#include "Objects2D.h"
#include "Physics2D.h"
#include "PlayerScoreGUI.h"
#include "GameStates.h"
#include "WindowInputHandlers.h"
#include "Network.h"

extern sf::RenderWindow mainRenderWindow;
extern sf::Font font;
extern sf::Vector2u windowSize;
extern GameState gameState;
extern WindowInputHandlers::KeyboardInputHandler keyboardHandler;
extern TCPnetwork::TCP_Base* network;

enum class Paddle {
	Left, Right
};

enum class PaddleDirection
{
	None, Up, Down
};

class Pong
{
protected:
	PlayerScoreGUI playerScore;
	sf::Clock clock;
	float timer = 0;
	const float platformSpeed = 400.f;
	float ballSpeed = 350.f;
	Objects2D::Platform firstPlatform, secondPlatform;
	Objects2D::Ball ball;
	Physics2D::Physics physics;
	sf::RectangleShape leftFieldSide, rightFieldSide;
	sf::CircleShape fieldCenterCircle;
	sf::Vertex line[2];

public:
	unsigned char playerID = 0;

protected:
	void ballBorderHit(BORDER border);

public:
	Pong();
	void initGame();
	void ResetGameClock();
	void movePaddle(Paddle paddle, PaddleDirection paddleDirection);
	virtual void UserInput();
	virtual void sendData() {}
	virtual void receiveData() {}
	virtual void GameUpdate();
	void Render();

};

class PongHost : public Pong
{
public:
	PongHost() { playerID = 1; }
	void sendData() override;
	void receiveData() override;
	void UserInput() override;
};

class PongClient : public Pong
{
private:
	PaddleDirection rightPaddledir = PaddleDirection::None;

public:
	PongClient() { playerID = 2;  }
	void GameUpdate() override;
	void sendData() override;
	void receiveData() override;
	void UserInput() override;
};
