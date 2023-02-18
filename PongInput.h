#pragma once

#include "Ping-Pong.h"
#include "GameStates.h"
#include "WindowInputHandlers.h"

extern Pong* pong;
extern GameState gameState;
extern WindowInputHandlers::KeyboardInputHandler keyboardHandler;

void leftPlayerInput();
void rightPlayerInput();
void handleInGameInput();
