#include "Ping-Pong.h"

void Pong::ballBorderHit(BORDER border)
{
    if (border == BORDER_LEFT)
        playerScore.addScore(PlayerScoreGUI::Player::SecondPlayer, 1);
    else if (border == BORDER_RIGHT)
        playerScore.addScore(PlayerScoreGUI::Player::FirstPlayer, 1);
}

Pong::Pong()
{
    srand(time(0));

    //windowSize = mainRenderWindow.getSize();
    //font.loadFromFile("Nautilus.otf");

    firstPlatform = Objects2D::Platform({ 0. + BorderThickness, 0. + BorderThickness }, { 25., 100. }, sf::Color::White);
    secondPlatform = Objects2D::Platform({ 600.f - 25.f - BorderThickness, 0. + BorderThickness }, { 25., 100. }, sf::Color::White);
    ball = Objects2D::Ball({ 300, 300 }, 10.f, sf::Color::Yellow);
    physics = Physics2D::Physics();

    playerScore.initScore();
    physics.randomizeBallAngle(ball);

    playerScore = PlayerScoreGUI(&font, 32, sf::Color::White);
    playerScore.firstPlayerScoreText.setPosition(sf::Vector2f(180.f, 0.f));
    playerScore.secondPlayerScoreText.setPosition(sf::Vector2f(420.f, 0.f));

    leftFieldSide.setSize(sf::Vector2f(windowSize.x / 2, windowSize.y));
    leftFieldSide.setFillColor(sf::Color(20, 160, 132));
    leftFieldSide.setPosition(sf::Vector2f(0.f, 0.f));

    rightFieldSide.setSize(sf::Vector2f(windowSize.x / 2, windowSize.y));
    rightFieldSide.setFillColor(sf::Color(38, 185, 153));
    rightFieldSide.setPosition(sf::Vector2f(windowSize.x / 2, 0.f));

    fieldCenterCircle.setRadius(50.f);
    fieldCenterCircle.setOrigin(sf::Vector2f(50.f, 50.f));
    fieldCenterCircle.setPosition(sf::Vector2f(windowSize.x / 2, windowSize.y / 2));
    fieldCenterCircle.setFillColor(sf::Color(255, 255, 255, 128));

    //unsigned short lineCount = windowSize.y / 20;
    line[0].color = sf::Color::White;
    line[1].color = sf::Color::White;
}

void Pong::initGame()
{
    firstPlatform.setCoords({ 0. + BorderThickness, 0. + BorderThickness });
    secondPlatform.setCoords({ 600.f - 25.f - BorderThickness, 0. + BorderThickness });
    ball.setCoords({ 300, 300 });
    playerScore.initScore();
    physics.randomizeBallAngle(ball);
    firstPlatform.setSpeed({0, 0});
    secondPlatform.setSpeed({0, 0});
    clock.restart();
}

void Pong::ResetGameClock()
{
    clock.restart();
}

void Pong::movePaddle(Paddle paddle, PaddleDirection paddleDirection)
{
    Objects2D::Platform* platform = nullptr;
    if (paddle == Paddle::Left)
        platform = &firstPlatform;
    else
        platform = &secondPlatform;

    if (paddleDirection == PaddleDirection::Up)
        platform->setSpeed({ 0.f, -1.f });
    else if (paddleDirection == PaddleDirection::Down)
        platform->setSpeed({ 0.f, 1.f });
    else
        platform->setSpeed({ 0.f, 0.f });
}

void Pong::UserInput()
{
    if (keyboardHandler.isPressed(sf::Keyboard::Enter))
        physics.randomizeBallAngle(ball);

    if (keyboardHandler.isPressed(sf::Keyboard::Num1))
        ballSpeed = 50.f;
    else if (keyboardHandler.isPressed(sf::Keyboard::Num2))
        ballSpeed = 150.f;
    else if (keyboardHandler.isPressed(sf::Keyboard::Num3))
        ballSpeed = 250.f;
    else if (keyboardHandler.isPressed(sf::Keyboard::Num4))
        ballSpeed = 350.f;

    if (keyboardHandler.isPressed(sf::Keyboard::W))
        firstPlatform.setSpeed({ 0.f, -1.f });
    else if (keyboardHandler.isPressed(sf::Keyboard::S))
        firstPlatform.setSpeed({ 0.f, 1.f });
    else
        firstPlatform.setSpeed({ 0.f, 0.f });

    if (keyboardHandler.isPressed(sf::Keyboard::Up))
        secondPlatform.setSpeed({ 0.f, -1.f });
    else if (keyboardHandler.isPressed(sf::Keyboard::Down))
        secondPlatform.setSpeed({ 0.f, 1.f });
    else
        secondPlatform.setSpeed({ 0.f, 0.f });

    return;
}

void Pong::GameUpdate()
{
    timer = clock.restart().asSeconds();

    //if (mainRenderWindow.hasFocus())
    //{
        BORDER collisionBorder = BORDER_NONE;

        collisionBorder = physics.resolveWindowFrameToObjectCollisions(firstPlatform, { static_cast<float>(windowSize.x), static_cast<float>(windowSize.y) });
        if (collisionBorder == BORDER_TOP || collisionBorder == BORDER_BOTTOM)
            firstPlatform.setSpeed({ 0.f, 0.f });


        collisionBorder = physics.resolveWindowFrameToObjectCollisions(secondPlatform, { static_cast<float>(windowSize.x), static_cast<float>(windowSize.y) });
        if (collisionBorder == BORDER_TOP || collisionBorder == BORDER_BOTTOM)
            secondPlatform.setSpeed({ 0.f, 0.f });

        collisionBorder = physics.resolveWindowFrameToObjectCollisions(ball, { static_cast<float>(windowSize.x), static_cast<float>(windowSize.y) });
        ballBorderHit(collisionBorder);

        physics.resolveBallToPlatformCollisions(ball, firstPlatform);
        physics.resolveBallToPlatformCollisions(ball, secondPlatform);

        physics.moveObject(firstPlatform, platformSpeed, timer);
        physics.moveObject(secondPlatform, platformSpeed, timer);
        physics.moveObject(ball, ballSpeed, timer);
        
        if (ballSpeed < 600.f)
            ballSpeed += 0.002;
    //}
}

void Pong::Render()
{
    //mainRenderWindow.clear(sf::Color(20, 160, 132));
    mainRenderWindow.draw(leftFieldSide);
    mainRenderWindow.draw(rightFieldSide);
    mainRenderWindow.draw(fieldCenterCircle);
    mainRenderWindow.draw(firstPlatform.getShape());
    mainRenderWindow.draw(secondPlatform.getShape());

    for (unsigned char i = 0; i < windowSize.y / 19; i++)
    {
        line[0].position = sf::Vector2f(windowSize.x / 2, i * 42);
        line[1].position = sf::Vector2f(windowSize.x / 2, i * 42 + 21);
        mainRenderWindow.draw(line, 2, sf::Lines);
    }

    mainRenderWindow.draw(ball.getShape());
    mainRenderWindow.draw(playerScore.firstPlayerScoreText);
    mainRenderWindow.draw(playerScore.secondPlayerScoreText);

    //mainRenderWindow.display();
}

///

void PongHost::sendData()
{
    static std::string data;
    data.clear();

    EGameState tmpGameState = gameState.getGameState();
    data.append((char*)&firstPlatform.getCoords(), sizeof(Points2D::Coords));
    data.append((char*)&secondPlatform.getCoords(), sizeof(Points2D::Coords));
    data.append((char*)&ball.getCoords(), sizeof(Points2D::Coords));
    data.append((char*)&playerScore.getScore(), sizeof(PlayerScore));
    data.append((char*)&tmpGameState, sizeof(EGameState));

    network->sendMessage(data);
}

void PongHost::receiveData()
{
    static std::string data;
    data.clear();

    network->receiveMessage(data);

    PaddleDirection paddleDir;
    EGameState tmpGameState;

    data.copy((char*)&paddleDir, sizeof(PaddleDirection));
    data.copy((char*)&tmpGameState, sizeof(EGameState), sizeof(PaddleDirection));
    if (tmpGameState != EGameState::InMultiplayer)
        gameState.setImmediately(tmpGameState);
    this->movePaddle(Paddle::Right, paddleDir);
}

void PongHost::UserInput()
{
    if (keyboardHandler.isPressed(sf::Keyboard::Up))
        firstPlatform.setSpeed({ 0.f, -1.f });
    else if (keyboardHandler.isPressed(sf::Keyboard::Down))
        firstPlatform.setSpeed({ 0.f, 1.f });
    else
        firstPlatform.setSpeed({ 0.f, 0.f });
}

///

void PongClient::GameUpdate()
{
    timer = clock.restart().asSeconds();
}

void PongClient::sendData()
{
    static std::string data;
    data.clear();

    EGameState tmpGameState = gameState.getGameState();
    data.append((char*)&rightPaddledir, sizeof(PaddleDirection));
    data.append((char*)&tmpGameState, sizeof(EGameState));
    
    network->sendMessage(data);
}

void PongClient::receiveData()
{
    static std::string data;
    data.clear();

    Points2D::Coords firstPlatformPos;
    Points2D::Coords secondPlatformPos;
    Points2D::Coords ballPos;
    PlayerScore receivedPlayerScore;
    EGameState tmpGameState;

    network->receiveMessage(data);
    unsigned int offset = 0;

    data.copy((char*)&firstPlatformPos, sizeof(Points2D::Coords), offset); 
    offset += sizeof(Points2D::Coords);

    data.copy((char*)&secondPlatformPos, sizeof(Points2D::Coords), offset);
    offset += sizeof(Points2D::Coords);

    data.copy((char*)&ballPos, sizeof(Points2D::Coords), offset);
    offset += sizeof(Points2D::Coords);

    data.copy((char*)&receivedPlayerScore, sizeof(PlayerScore), offset);
    offset += sizeof(PlayerScore);

    data.copy((char*)&tmpGameState, sizeof(EGameState), offset);
    offset += sizeof(EGameState);

    firstPlatform.setCoords(firstPlatformPos);
    secondPlatform.setCoords(secondPlatformPos);
    ball.setCoords(ballPos);

    if (tmpGameState != EGameState::InMultiplayer)
        gameState.setImmediately(tmpGameState);
    
    PlayerScore score = playerScore.getScore();
    if (score.firstPlayerScore != receivedPlayerScore.firstPlayerScore)
    {
        playerScore.setScore(
            PlayerScoreGUI::Player::FirstPlayer, 
            receivedPlayerScore.firstPlayerScore
        );
    }

    if (score.secondPlayerScore != receivedPlayerScore.secondPlayerScore)
    {
        playerScore.setScore(
            PlayerScoreGUI::Player::SecondPlayer,
            receivedPlayerScore.secondPlayerScore
        );
    }
}

void PongClient::UserInput()
{
    if (keyboardHandler.isPressed(sf::Keyboard::Up))
        rightPaddledir = PaddleDirection::Up;
    else if (keyboardHandler.isPressed(sf::Keyboard::Down))
        rightPaddledir = PaddleDirection::Down;
    else
        rightPaddledir = PaddleDirection::None;
}
