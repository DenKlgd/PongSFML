#pragma once
#include "SFML/Graphics.hpp"
#include "PlayerScore.h"

class PlayerScoreGUI
{
private:
	PlayerScore playerScore;
	sf::Font* font = nullptr;
	
private:
	void initText(sf::Text& text, unsigned int fontSize, sf::Color fillColor)
	{
		text.setFont(*font);
		text.setCharacterSize(fontSize);
		text.setFillColor(fillColor);
	}

	void scoreToText(sf::Text& playerScoreText, Score score)
	{
		playerScoreText.setString(std::to_string(score));
	}

public:
	sf::Text firstPlayerScoreText;
	sf::Text secondPlayerScoreText;

public:
	enum struct Player : unsigned char
	{
		FirstPlayer, SecondPlayer
	};

	PlayerScoreGUI() = default;

	PlayerScoreGUI(sf::Font* font, unsigned int fontSize, sf::Color fillColor)
	{
		this->font = font;
		initText(firstPlayerScoreText, fontSize, fillColor);
		initText(secondPlayerScoreText, fontSize, fillColor);
		scoreToText(firstPlayerScoreText, playerScore.firstPlayerScore);
		scoreToText(secondPlayerScoreText, playerScore.secondPlayerScore);
	}

	void addScore(Player player, Score amount)
	{
		switch (player)
		{
		case Player::FirstPlayer:
			playerScore.firstPlayerScore += amount;
			scoreToText(firstPlayerScoreText, playerScore.firstPlayerScore);
			break;
		case Player::SecondPlayer:
			playerScore.secondPlayerScore += amount;
			scoreToText(secondPlayerScoreText, playerScore.secondPlayerScore);
			break;
		default:
			break;
		}
	}

	void setScore(Player player, Score amount)
	{
		switch (player)
		{
		case Player::FirstPlayer:
			playerScore.firstPlayerScore = amount;
			scoreToText(firstPlayerScoreText, playerScore.firstPlayerScore);
			break;
		case Player::SecondPlayer:
			playerScore.secondPlayerScore = amount;
			scoreToText(secondPlayerScoreText, playerScore.secondPlayerScore);
			break;
		default:
			break;
		}
	}

	void initScore()
	{
		playerScore.initScore();
		scoreToText(firstPlayerScoreText, playerScore.firstPlayerScore);
		scoreToText(secondPlayerScoreText, playerScore.secondPlayerScore);
	}

	PlayerScore getScore()
	{
		return playerScore;
	}
};