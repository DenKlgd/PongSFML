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

void KeyboardInputHandler::resetPressedKeys()
{
	for (auto& key : pressedKeys)
		key.isActive = false;
	lastKeyIndex = 0;
}

///

