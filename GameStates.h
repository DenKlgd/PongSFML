#pragma once

enum class EGameState : unsigned char
{
	Menu,
	MultiplayerMenu,
	ConnectMenu,
	InGame,
	InMultiplayer,
	Pause,
	PauseMultiplayer,
	WaitingForOtherPlayer,
	JoiningToHost,
	SETTINGS,
};

class GameState
{
private:
	EGameState _gameState = EGameState::Menu;
	EGameState _requestedGameState = _gameState;

public:
	GameState() = default;
	GameState(EGameState);
	void requestGameStateChange(EGameState);
	void applyGameStateChange();
	void setImmediately(EGameState);
	EGameState getGameState();
};
