#include "PongInput.h"

void leftPlayerInput()
{
    if (keyboardHandler.isPressed(sf::Keyboard::W))
        pong->movePaddle(Paddle::Left, PaddleDirection::Up);
    else if (keyboardHandler.isPressed(sf::Keyboard::S))
        pong->movePaddle(Paddle::Left, PaddleDirection::Down);
    else
        pong->movePaddle(Paddle::Left, PaddleDirection::None);
}

void rightPlayerInput()
{
    if (keyboardHandler.isPressed(sf::Keyboard::Up))
        pong->movePaddle(Paddle::Right, PaddleDirection::Up);
    else if (keyboardHandler.isPressed(sf::Keyboard::Down))
        pong->movePaddle(Paddle::Right, PaddleDirection::Down);
    else
        pong->movePaddle(Paddle::Right, PaddleDirection::None);
}

void handleInGameInput()
{
    leftPlayerInput();
    rightPlayerInput();
}
