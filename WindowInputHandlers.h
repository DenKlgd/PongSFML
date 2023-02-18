#pragma once
#include "SFML/Window/Keyboard.hpp"

#define PRESSED_KEYS_ARRAY_LENGTH 8

namespace WindowInputHandlers
{
	class KeyboardInputHandler
	{
	private:
		struct 
		{
			sf::Keyboard::Key key;
			bool isActive = false;
		} pressedKeys[PRESSED_KEYS_ARRAY_LENGTH];
		unsigned char lastKeyIndex = 0;

	private:
		unsigned char getFirstEmptyIndex();
		unsigned char checkKeyInArray(const sf::Keyboard::Key key);

	public:
		KeyboardInputHandler() = default;
		bool pushKey(const sf::Keyboard::Key key);
		bool popKey(const sf::Keyboard::Key key);
		bool isPressed(const sf::Keyboard::Key key);
		sf::Keyboard::Key getLastKey();

	};

	char mapKeyToChar(sf::Keyboard::Key);
}
