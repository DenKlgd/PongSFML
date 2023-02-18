#include "GUI.h"

using namespace GUI;
using namespace Points2D;

//void GUIelement::setCoords(Points2D::Coords coords)
//{
//	this->coords = coords;
//}

Coords GUIelement::getCoords()
{
	return coords;
}

Size GUIelement::getSize()
{
	return size;
}

void GUIelement::update()
{
	return;
}

bool GUIelement::userInput(sf::Event& event)
{
	return false;
}

///

void ShapeGUIelement::setCoords(Points2D::Coords coords)
{
	this->coords = coords;
	this->shape->setPosition(coords.x, coords.y);
}

void ShapeGUIelement::setColor(sf::Color fillColor, sf::Color outlineColor)
{
	this->fillColor = fillColor;
	this->outlineColor = outlineColor;
	this->shape->setFillColor(fillColor);
	this->shape->setOutlineColor(outlineColor);
}

///

void ImageGUIelement::calcSize()
{
	sf::FloatRect rect = _sprite.getLocalBounds();
	size = { rect.width, rect.height };
	_sprite.setOrigin(size.x / 2.f, size.y / 2.f);
}

void ImageGUIelement::setCoords(Points2D::Coords coords)
{
	this->coords = coords;
	_sprite.setOrigin(0, 0);
	_sprite.setPosition(coords.x, coords.y);
	calcSize();
}

void ImageGUIelement::setSize(Points2D::Size size)
{
	return;
}

void ImageGUIelement::setSpriteTexture(sf::Texture& texture)
{
	_sprite.setTexture(texture);
	calcSize();
}

void ImageGUIelement::setRotation(float rotationAngle)
{
	_rotationAngle = rotationAngle;
}

void ImageGUIelement::setScale(float x, float y)
{
	_sprite.setScale(x, y);
}

void ImageGUIelement::draw(sf::RenderWindow& renderWindow)
{
	renderWindow.draw(_sprite);
}

void ImageGUIelement::update()
{
	if (_timer.getElapsedTime().asMilliseconds() <= 25)
		return;

	_sprite.rotate(_rotationAngle);
	_timer.restart();
}

ImageGUIelement::ImageGUIelement()
{
	type = ElementType::IMAGEBOX;
}

/// 

void Label::alignText()
{
	float centeredX = (coords.x + size.x / 2) - (text.getString().getSize() * text.getCharacterSize() / 5);
	text.setPosition(centeredX, coords.y);
}

void Label::setCoords(Points2D::Coords coords)
{
	this->coords = coords;
	this->shape->setPosition(coords.x, coords.y);
	alignText();
}

void Label::setSize(Points2D::Size size)
{
	this->size = size;
	dynamic_cast<sf::RectangleShape*>(this->shape)->setSize({size.x, size.y});
	this->alignText();
}

void Label::setText(sf::String text)
{
	this->text.setString(text);
	this->alignText();
}

void Label::setCharSize(unsigned int charSize)
{
	this->text.setCharacterSize(charSize);
	this->alignText();
}

bool Label::setFont(sf::Font* font)
{
	if (!font)
		return false;
	this->text.setFont(*font);
	return true;
}

void Label::draw(sf::RenderWindow& renderWindow)
{
	renderWindow.draw(*shape);
	renderWindow.draw(text);
}

bool Label::userInput(sf::Event& event)
{
	return false;
}

Label::Label()
{
	type = ElementType::LABEL;
	shape = new sf::RectangleShape;
	sf::RectangleShape* rect = dynamic_cast<sf::RectangleShape*>(shape);
	rect->setPosition(coords.x, coords.y);
	rect->setSize({ size.x, size.y });
	rect->setFillColor(fillColor);
	rect->setOutlineColor(outlineColor);
	rect->setOutlineThickness(2);
	text.setColor(sf::Color::Black);
}

Label::~Label()
{
	delete dynamic_cast<sf::RectangleShape*>(shape);
	shape = nullptr;
}

///

void Button::setInteractiveColors(sf::Color mouseOverColor, sf::Color mousePressedColor)
{
	this->mouseOverColor = mouseOverColor;
	this->mousePressedColor = mousePressedColor;
}

bool Button::userInput(sf::Event& event)
{
	Points2D::Point2D mousePos;
	if (event.type == sf::Event::MouseMoved)
	{
		mousePos.x = (float)event.mouseMove.x;
		mousePos.y = (float)event.mouseMove.y;
	}
	else if (event.mouseButton.button == sf::Mouse::Left)
	{
		mousePos.x = (float)event.mouseButton.x;
		mousePos.y = (float)event.mouseButton.y;
	}
	else
		return false;

	if (mousePos.x >= coords.x && mousePos.x <= coords.x + size.x
		&&
		mousePos.y >= coords.y && mousePos.y <= coords.y + size.y
	)
	{
		if (event.type == sf::Event::MouseMoved && state != ElementState::Active)
		{
			if (state == ElementState::MouseOver)
				return false;
			state = ElementState::MouseOver;
			shape->setFillColor(mouseOverColor);
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			if (state == ElementState::Active)
				return false;
			state = ElementState::Active;
			shape->setFillColor(mousePressedColor);
		}
		else if (event.type == sf::Event::MouseButtonReleased && state == ElementState::Active)
		{
			if (state == ElementState::MouseOver)
				return true;
			state = ElementState::MouseOver;
			shape->setFillColor(mouseOverColor);
			return true;
		}
	}
	else
	{
		if (event.type == sf::Event::MouseButtonReleased
			||
			state != ElementState::Active
		)
		{
			if (state == ElementState::Default)
				return false;
			state = ElementState::Default;
			shape->setFillColor(fillColor);
		}
	}
	return false;
}

Button::Button()
{
	type = ElementType::BUTTON;
}

///

void InputBox::removeUnderscore(bool updateScreenText)
{
	if (addUnderScore)
	{
		addUnderScore = false;
		size_t pos = _textString.rfind(L'_');
		if (pos != std::string::npos)
		{
			_textString.erase(pos);
			if (updateScreenText)
				text.setString(_textString);
		}
	}
}

void InputBox::blinkUnderscore()
{
	if (state != ElementState::Active)
	{
		removeUnderscore(true);
		if (_textString.length())
			return;
		text.setString(_captionString);
		alignText();
		return;
	}

	if (_timer.getElapsedTime().asSeconds() < 0.5f)
	{
		if (!addUnderScore)
		{
			addUnderScore = true;
			_textString += L'_';
			text.setString(_textString);
		}
	}
	else
	{
		removeUnderscore(true);
	}

	if (_timer.getElapsedTime().asSeconds() >= 1.f)
		_timer.restart();
}

InputBox::InputBox()
{
	type = ElementType::INPUTBOX;
}

void InputBox::setMaxTextLength(uint16_t maxTextLength)
{
	_maxTextLength = maxTextLength;
	//_textString.resize(maxTextLength);
}

void InputBox::update()
{
	blinkUnderscore();
}

bool InputBox::userInput(sf::Event& event)
{
	if (state == ElementState::Active && event.type == sf::Event::TextEntered)
	{
		removeUnderscore(false);

		if (keyboard->isPressed(sf::Keyboard::Enter))
		{
			state = ElementState::Default;
			return false;
		}
		else if (keyboard->isPressed(sf::Keyboard::BackSpace))
		{
			if (_textString.length() > 0)
			{
				_textString.resize(_textString.length() - 1);
				text.setString(_textString);
				alignText();
			}
			return false;
		}

		if (_textString.length() >= _maxTextLength)
			return false;

		if (!filterFunction(event.text.unicode))
			return false;
		_textString += event.text.unicode;
		text.setString(_textString);
		alignText();
	}

	Points2D::Point2D mousePos;
	if (event.type == sf::Event::MouseMoved)
	{
		mousePos.x = (float)event.mouseMove.x;
		mousePos.y = (float)event.mouseMove.y;
	}
	else if (event.mouseButton.button == sf::Mouse::Left)
	{
		mousePos.x = (float)event.mouseButton.x;
		mousePos.y = (float)event.mouseButton.y;
	}
	else
		return false;

	if (mousePos.x >= coords.x && mousePos.x <= coords.x + size.x
		&&
		mousePos.y >= coords.y && mousePos.y <= coords.y + size.y
	)
	{
		if (event.type == sf::Event::MouseMoved && state != ElementState::Active)
		{
			if (state == ElementState::MouseOver)
				return false;
			state = ElementState::MouseOver;
			shape->setOutlineThickness(4);
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			if (state == ElementState::Active)
				return false;
			state = ElementState::Active;
			shape->setOutlineThickness(6);
		}
		else if (event.type == sf::Event::MouseButtonReleased && state == ElementState::Active)
		{
			_timer.restart();
			return true;
		}
	}
	else
	{
		if (event.type == sf::Event::MouseButtonReleased
			||
			state != ElementState::Active
		)
		{
			if (state == ElementState::Default)
				return false;
			state = ElementState::Default;
			shape->setOutlineThickness(2);
		}
	}

	return false;
}

const std::wstring& InputBox::getText() const
{
	return _textString;
}

///

std::vector<MenuElement>::iterator Menu::checkIfElementExists(ElementID elementID)
{
	std::vector<MenuElement>::iterator iter = menuElements.begin();
	std::vector<MenuElement>::iterator iterEnd = menuElements.end();

	for (; iter < iterEnd; iter++)
	{
		if ((*iter).elementID == elementID)
		{
			return iter;
		}
	}

	return iterEnd;
}

GUIelement* Menu::allocMemory(ElementType type, ElementID elementID)
{
	std::vector<MenuElement>::iterator iterEnd = menuElements.end();
	std::vector<MenuElement>::iterator iter = checkIfElementExists(elementID);
	if (iter != iterEnd)
		return nullptr;

	GUIelement* element = nullptr;

	switch (type)
	{
	case ElementType::LABEL:
		element = new Label;
		break;
	case ElementType::BUTTON:
		element = new Button;
		break;
	case ElementType::INPUTBOX:
		element = new InputBox;
		break;
	case ElementType::IMAGEBOX:
		element = new ImageGUIelement;
		break;
	default:
		return nullptr;
	}

	MenuElement menuElement;
	menuElement.guiElement = element;
	menuElement.elementID = elementID;

	menuElements.push_back(menuElement);
	return element;
}

Menu::Menu() 
{

}

bool Menu::loadFont(sf::Font* font)
{
	if (font == nullptr)
		return false;

	this->font = font;
	return true;
}

bool Menu::addLabel(sf::String text, Points2D::Coords coords, Points2D::Size size, ElementID elementID)
{
	Label* label = (Label*)allocMemory(ElementType::LABEL, elementID);
	
	if (label == nullptr)
		return false;

	label->setText(text);
	label->setCharSize(32);
	label->setColor(sf::Color::White, sf::Color::Black);
	label->setFont(font);
	label->setCoords(coords);
	label->setSize(size);

	return true;
}

bool Menu::addButton(sf::String text, Points2D::Coords coords, Points2D::Size size, ElementID elementID)
{
	Button* button = (Button*)allocMemory(ElementType::BUTTON, elementID);

	if (button == nullptr)
		return false;

	button->setText(text);
	button->setCharSize(32);
	button->setColor(sf::Color::White, sf::Color::Black);
	button->setFont(font);
	button->setCoords(coords);
	button->setSize(size);

	return true;
}

bool Menu::addInputBox(Points2D::Coords coords, Points2D::Size size, ElementID elementID, uint16_t maxStrLen, std::wstring startText)
{
	InputBox* inputBox = (InputBox*)allocMemory(ElementType::INPUTBOX, elementID);

	if (inputBox == nullptr)
		return false;

	inputBox->_captionString = std::move(startText);
	inputBox->setText(std::move(startText));
	inputBox->setMaxTextLength(maxStrLen);
	inputBox->setCharSize(32);
	inputBox->setColor(sf::Color::White, sf::Color::Black);
	inputBox->setFont(font);
	inputBox->setCoords(coords);
	inputBox->setSize(size);

	return true;
}

bool Menu::addImageBox(Points2D::Coords coords, sf::Texture& texture, ElementID elementID)
{
	ImageGUIelement* imageBox = (ImageGUIelement*)allocMemory(ElementType::IMAGEBOX, elementID);

	if (imageBox == nullptr)
		return false;

	imageBox->setCoords(coords);
	imageBox->setSpriteTexture(texture);

	return true;
}

bool Menu::removeElement(ElementID elementID)
{
	std::vector<MenuElement>::iterator iterEnd = menuElements.end();
	std::vector<MenuElement>::iterator iter = checkIfElementExists(elementID);

	if (iter == iterEnd)
		return false;

	delete (*iter).guiElement;
	menuElements.erase(iter);
	return true;
}

void Menu::alignElementsCenter(sf::Vector2u windowSize)
{
	std::vector<MenuElement>::iterator iter = menuElements.begin();
	std::vector<MenuElement>::iterator iterEnd = menuElements.end();

	Coords baseCoords;
	baseCoords.x = windowSize.x / 2;
	baseCoords.y = windowSize.y / 2;

	while (iter != iterEnd)
	{
		Size objSize = iter->guiElement->getSize();
		Coords objCoords = iter->guiElement->getCoords();
		iter->guiElement->setCoords({baseCoords.x - objSize.x / 2, objCoords.y});
		++iter;
	}
}

ElementID Menu::userInput(sf::Event& event)
{
	std::vector<MenuElement>::iterator iter = menuElements.begin();
	std::vector<MenuElement>::iterator iterEnd = menuElements.end();

	while (iter != iterEnd)
	{
		if (iter->guiElement->userInput(event))
			return iter->elementID;
		++iter;
	}
	return -1;
}

void Menu::update()
{
	for (auto& it : menuElements)
	{
		it.guiElement->update();
	}
}

void Menu::draw()
{
	if (!renderWindow || !font)
		return;

	std::vector<MenuElement>::iterator iter = menuElements.begin();
	std::vector<MenuElement>::iterator iterEnd = menuElements.end();

	while (iter != iterEnd)
	{
		iter->guiElement->draw(*renderWindow);
		++iter;
	}
}

GUIelement* Menu::getElementByID(ElementID elementID)
{
	std::vector<MenuElement>::iterator iter = checkIfElementExists(elementID);
	std::vector<MenuElement>::iterator iterEnd = menuElements.end();

	if (iter == iterEnd)
		return nullptr;
	return iter->guiElement;
}

Menu::~Menu()
{
	size_t size = menuElements.size();
	std::vector<MenuElement>::iterator iter = menuElements.begin();
	std::vector<MenuElement>::iterator iterEnd = menuElements.end();

	if (!size)
		return;

	while (iter != iterEnd)
	{
		delete iter->guiElement;
		++iter;
	}
}
