#include "GUI.h"

using namespace GUI;
using namespace Points2D;


bool GUIelement::getMousePos(sf::Event& event, Points2D::Point2D& mousePos)
{
	if (event.type == sf::Event::MouseMoved)
	{
		mousePos.x = (float)event.mouseMove.x;
		mousePos.y = (float)event.mouseMove.y;
		return true;
	}
	else if (event.mouseButton.button == sf::Mouse::Left)
	{
		mousePos.x = (float)event.mouseButton.x;
		mousePos.y = (float)event.mouseButton.y;
		return true;
	}
	return false;
}

ElementState GUIelement::processEvent(sf::Event& event)
{
	sf::FloatRect btnRect;

	if (dynamic_cast<Button*>(this) || dynamic_cast<GUI::InputBox*>(this))
		btnRect = ((sf::RectangleShape*)drawable)->getGlobalBounds();
	else if (dynamic_cast<ImageButton*>(this))
		btnRect = ((sf::Sprite*)drawable)->getGlobalBounds();
	else
		return ElementState::NoRedraw;

	Points2D::Point2D mousePos;
	if (!getMousePos(event, mousePos))
		return ElementState::NoRedraw;

	if (mousePos.x >= btnRect.left && mousePos.x <= btnRect.left + btnRect.width
		&&
		mousePos.y >= btnRect.top && mousePos.y <= btnRect.top + btnRect.height
		)
	{
		if (event.type == sf::Event::MouseMoved && state != ElementState::Active)
		{
			state = ElementState::MouseOver;
			return ElementState::MouseOver;
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			state = ElementState::Active;
			return ElementState::Active;
		}
		else if (event.type == sf::Event::MouseButtonReleased && state == ElementState::Active)
		{
			state = ElementState::MouseOver;
			return ElementState::Clicked;
		}
	}
	else
	{
		if (event.type == sf::Event::MouseButtonReleased
			||
			state != ElementState::Active
			)
		{
			state = ElementState::Default;
			return ElementState::Default;
		}
	}
	return ElementState::NoRedraw;
}

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

void ShapeGUIelement::setColor(sf::Color fillColor, sf::Color outlineColor)
{
	this->fillColor = fillColor;
	this->outlineColor = outlineColor;
	((sf::RectangleShape*)drawable)->setFillColor(fillColor);
	((sf::RectangleShape*)drawable)->setOutlineColor(outlineColor);
}

///

void ImageGUIelement::calcSize()
{
	sf::Sprite* sprite = (sf::Sprite*)drawable;
	sf::FloatRect rect = sprite->getLocalBounds();
	size = { rect.width, rect.height };
	sprite->setOrigin(size.x / 2.f, size.y / 2.f);
}

void ImageGUIelement::setCoords(Points2D::Coords coords)
{
	this->coords = coords;
	sf::Sprite* sprite = (sf::Sprite*)drawable;
	sprite->setOrigin(0, 0);
	sprite->setPosition(coords.x, coords.y);
	calcSize();
}

void ImageGUIelement::setSize(Points2D::Size size)
{
	return;
}

void ImageGUIelement::setSpriteTexture(sf::Texture& texture)
{
	((sf::Sprite*)drawable)->setTexture(texture);
	calcSize();
}

void ImageGUIelement::rotate(float angle)
{
	((sf::Sprite*)drawable)->rotate(angle);
}

void ImageGUIelement::setRotation(float rotationAngle)
{
	_rotationAngle = rotationAngle;
}

void ImageGUIelement::setScale(float x, float y)
{
	((sf::Sprite*)drawable)->setScale(x, y);
	calcSize();
}

void ImageGUIelement::draw(sf::RenderWindow& renderWindow)
{
	renderWindow.draw(*((sf::Sprite*)drawable));
}

void ImageGUIelement::update()
{
	if (_timer.getElapsedTime().asMilliseconds() <= 25)
		return;

	((sf::Sprite*)drawable)->rotate(_rotationAngle);
	_timer.restart();
}

void ImageGUIelement::rescale(float scaleX, float scaleY)
{
	this->coords.x *= scaleX;
	this->coords.y *= scaleY;
	sf::Vector2f scale = ((sf::Sprite*)drawable)->getScale();
	scale.x *= scaleX;
	scale.y *= scaleY;
	((sf::Sprite*)drawable)->setScale(scale.x, scale.x);
	this->setCoords(this->coords);
}

ImageGUIelement::ImageGUIelement()
{
	type = ElementType::IMAGEBOX;
	drawable = new sf::Sprite;
}

ImageGUIelement::~ImageGUIelement()
{
	delete drawable;
	drawable = nullptr;
}

/// 

void Label::alignText()
{
	float centeredX = (coords.x + size.x / 2) - (text.getLocalBounds().width / 2);
	text.setPosition(centeredX, coords.y);
}

void Label::setCoords(Points2D::Coords coords)
{
	this->coords = coords;
	((sf::RectangleShape*)drawable)->setPosition(coords.x, coords.y);
	alignText();
}

void Label::setSize(Points2D::Size size)
{
	this->size = size;
	((sf::RectangleShape*)drawable)->setSize({size.x, size.y});
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
	renderWindow.draw(*((sf::RectangleShape*)drawable));
	renderWindow.draw(text);
}

bool Label::userInput(sf::Event& event)
{
	return false;
}

void Label::rescale(float scaleX, float scaleY)
{
	this->coords.x *= scaleX;
	this->coords.y *= scaleY;
	this->size.x *= scaleX;
	this->size.y *= scaleY;
	((sf::RectangleShape*)drawable)->setPosition(coords.x, coords.y);
	((sf::RectangleShape*)drawable)->setSize({ size.x, size.y });
	text.setCharacterSize(text.getCharacterSize() * scaleX);
	alignText();
}

Label::Label()
{
	type = ElementType::LABEL;
	drawable = new sf::RectangleShape;
	sf::RectangleShape* rect = dynamic_cast<sf::RectangleShape*>(drawable);
	rect->setPosition(coords.x, coords.y);
	rect->setSize({ size.x, size.y });
	rect->setFillColor(fillColor);
	rect->setOutlineColor(outlineColor);
	rect->setOutlineThickness(2);
	text.setColor(sf::Color::Black);
}

Label::~Label()
{
	delete ((sf::RectangleShape*)drawable);
	drawable = nullptr;
}

///

void Button::setInteractiveColors(sf::Color mouseOverColor, sf::Color mousePressedColor)
{
	this->mouseOverColor = mouseOverColor;
	this->mousePressedColor = mousePressedColor;
}

bool Button::userInput(sf::Event& event)
{
	ElementState state = processEvent(event);

	switch (state)
	{
	case GUI::ElementState::NoRedraw:
		return false;
	case GUI::ElementState::Default:
		((sf::RectangleShape*)drawable)->setFillColor(fillColor);
		break;
	case GUI::ElementState::MouseOver:
		((sf::RectangleShape*)drawable)->setFillColor(mouseOverColor);
		break;
	case GUI::ElementState::Active:
		((sf::RectangleShape*)drawable)->setFillColor(mousePressedColor);
		break;
	case GUI::ElementState::Clicked:
		((sf::RectangleShape*)drawable)->setFillColor(mouseOverColor);
		return true;
	default:
		break;
	}
	return false;
}

Button::Button()
{
	type = ElementType::BUTTON;
}

///

void ImageButton::calcSize()
{
	(*this).ImageGUIelement::calcSize();
	frame.setSize({ size.x + 2.f, size.y + 2.f });
}

void ImageButton::setCoords(Points2D::Coords coords)
{
	(*this).ImageGUIelement::setCoords(coords);
	calcSize();
	frame.setSize({ size.x + 2.f, size.y + 2.f });
	frame.setPosition({ coords.x - 1.f, coords.y - 1.f});
}

void ImageButton::setScale(float x, float y)
{
	((sf::Sprite*)drawable)->setScale(x, y);
	frame.setScale(x, y);
	frame.setPosition(((sf::Sprite*)drawable)->getGlobalBounds().left - 1.f, ((sf::Sprite*)drawable)->getGlobalBounds().top - 1.f);
	calcSize();
}

bool ImageButton::userInput(sf::Event& event)
{
	ElementState state = processEvent(event);

	switch (state)
	{
	case GUI::ElementState::NoRedraw:
		return false;
	case GUI::ElementState::Default:
		frame.setOutlineThickness(0);
		break;
	case GUI::ElementState::MouseOver:
		frame.setOutlineThickness(2);
		break;
	case GUI::ElementState::Active:
		frame.setOutlineThickness(4);
		break;
	case GUI::ElementState::Clicked:
		frame.setOutlineThickness(2);
		return true;
	default:
		break;
	}
	return false;
}

void ImageButton::draw(sf::RenderWindow& renderWindow)
{
	renderWindow.draw(*drawable);
	renderWindow.draw(frame);
}

void ImageButton::rescale(float scaleX, float scaleY)
{
	coords.x *= scaleX; coords.y *= scaleY;
	((sf::Sprite*)drawable)->setPosition({ coords.x, coords.y });
	
	((sf::Sprite*)drawable)->scale(scaleX, scaleY);
	frame.scale(scaleX, scaleY);

	frame.setPosition(((sf::Sprite*)drawable)->getGlobalBounds().left - 1.f, ((sf::Sprite*)drawable)->getGlobalBounds().top - 1.f);	
}

ImageButton::ImageButton()
{
	type = ElementType::IMAGEBUTTON;
	frame.setOutlineColor(sf::Color::Black);
	frame.setFillColor(sf::Color(0, 0, 0, 0));
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
}

void InputBox::update()
{
	blinkUnderscore();
}

bool InputBox::userInput(sf::Event& event)
{
	if (state == ElementState::Active && event.type == sf::Event::TextEntered)
	{
		if (keyboard->isPressed(sf::Keyboard::Enter))
		{
			state = ElementState::Default;
			((sf::RectangleShape*)drawable)->setOutlineThickness(2);
			removeUnderscore(true);
			return false;
		}
		else if (keyboard->isPressed(sf::Keyboard::BackSpace))
		{
			removeUnderscore(false);
			if (_textString.length() > 0)
			{
				_textString.resize(_textString.length() - 1);
				text.setString(_textString);
				alignText();
			}
			return false;
		}

		removeUnderscore(false);

		if (_textString.length() >= _maxTextLength)
			return false;

		if (!filterFunction(event.text.unicode))
			return false;
		_textString += event.text.unicode;
		text.setString(_textString);
		alignText();
	}


	ElementState state = processEvent(event);

	switch (state)
	{
	case GUI::ElementState::NoRedraw:
		return false;
	case GUI::ElementState::Default:
		((sf::RectangleShape*)drawable)->setOutlineThickness(2);
		break;
	case GUI::ElementState::MouseOver:
		((sf::RectangleShape*)drawable)->setOutlineThickness(4);
		break;
	case GUI::ElementState::Active:
		((sf::RectangleShape*)drawable)->setOutlineThickness(6);
		break;
	case GUI::ElementState::Clicked:
		this->state = GUI::ElementState::Active;
		_timer.restart();
		return true;
	default:
		break;
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
	case ElementType::IMAGEBUTTON:
		element = new ImageButton;
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

bool Menu::addImageButton(Points2D::Coords coords, sf::Texture& texture, ElementID elementID)
{
	ImageGUIelement* imageButton = (ImageGUIelement*)allocMemory(ElementType::IMAGEBUTTON, elementID);

	if (imageButton == nullptr)
		return false;

	imageButton->setCoords(coords);
	imageButton->setSpriteTexture(texture);

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

void Menu::setScale(float scaleX, float scaleY)
{
	std::vector<MenuElement>::iterator iter = menuElements.begin();
	while (iter != menuElements.end())
	{
		iter->guiElement->rescale(scaleX, scaleY);
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
