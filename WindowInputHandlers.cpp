#include "WindowInputHandlers.h"

using namespace WindowInputHandlers;

unsigned char KeyboardInputHandler::getFirstEmptyIndex()
{
	for (unsigned char i = 0; i < PRESSED_KEYS_ARRAY_LENGTH; i++)
	{
		if (!pressedKeys[i].isActive)
			return i;
	}
	return -1;
}

unsigned char KeyboardInputHandler::checkKeyInArray(const sf::Keyboard::Key key)
{
	for (unsigned char i = 0; i < PRESSED_KEYS_ARRAY_LENGTH; i++)
	{
		if (pressedKeys[i].isActive && pressedKeys[i].key == key)
			return i;
	}
	return -1;
}

bool KeyboardInputHandler::pushKey(const sf::Keyboard::Key key)
{
	if (checkKeyInArray(key) != unsigned char(-1))
		return false;

	unsigned char index = getFirstEmptyIndex();
	if (index == unsigned char(-1))
		return false;

	pressedKeys[index].isActive = true;
	pressedKeys[index].key = key;
	lastKeyIndex = index;
	
	return true;
}

bool KeyboardInputHandler::popKey(const sf::Keyboard::Key key)
{
	unsigned char index = checkKeyInArray(key);
	if (index == unsigned char(-1))
		return false;

	pressedKeys[index].isActive = false;

	return true;
}

bool KeyboardInputHandler::isPressed(const sf::Keyboard::Key key)
{
	if (checkKeyInArray(key) != unsigned char(-1))
		return true;

	return false;
}

sf::Keyboard::Key KeyboardInputHandler::getLastKey()
{
	return pressedKeys[lastKeyIndex].key;
}

///

char WindowInputHandlers::mapKeyToChar(sf::Keyboard::Key key)
{
	char letter = static_cast<char>(key);
	if (letter >= 0 && letter <= 25)
		letter = 'a' + letter;
	else if (letter >= 26 && letter <= 35)
	{
		letter -= 26;
		letter = '0' + letter;
	}
	else if (letter >= 75 && letter <= 84)
	{
		letter -= 75;
		letter = '0' + letter;
	}
	else
		letter = '\0';

	return letter;
}
