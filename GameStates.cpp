#include "GameStates.h"

GameState::GameState(EGameState gameState)
{
	setImmediately(gameState);
}

void GameState::requestGameStateChange(EGameState requestedGameState)
{
	_requestedGameState = requestedGameState;
}

void GameState::applyGameStateChange()
{
	_gameState = _requestedGameState;
}

void GameState::setImmediately(EGameState gameState)
{
	_gameState = gameState;
	_requestedGameState = gameState;
}

EGameState GameState::getGameState()
{
	return _gameState;
}
