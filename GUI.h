#pragma once
#include <vector>
#include "Point2D.h"
#include "SFML/Graphics.hpp"
#include "WindowInputHandlers.h"

namespace GUI {
	typedef short ElementID, TabIndex;
	enum class ElementType
	{
		NONE,
		LABEL,
		BUTTON,
		INPUTBOX,
		IMAGEBOX,
	};
	enum class ElementState
	{
		Default,
		MouseOver,
		Active,
	};

	class GUIelement
	{
	protected:
		ElementState state = ElementState::Default;
		ElementType type = ElementType::NONE;
		Points2D::Coords coords;
		Points2D::Size size;
	public:
		virtual void setCoords(Points2D::Coords coords) = 0;
		virtual void setSize(Points2D::Size size) = 0;
		Points2D::Coords getCoords();
		Points2D::Size getSize();
		virtual void draw(sf::RenderWindow& renderWindow) = 0;
		virtual void update();
		virtual bool userInput(sf::Event& event);
		virtual ~GUIelement() = default;
	};

	class ShapeGUIelement : public GUIelement
	{
	protected:
		sf::Shape* shape = nullptr;
		sf::Color fillColor = sf::Color::White;
		sf::Color outlineColor = sf::Color::Black;
	public:
		void setCoords(Points2D::Coords coords) override;
		void setColor(sf::Color fillColor, sf::Color outlineColor);
	};

	class ImageGUIelement : public GUIelement
	{
	protected:
		sf::Sprite _sprite;
		float _rotationAngle = 0.f;
		sf::Clock _timer;
	protected:
		void calcSize();
	public:
		void setCoords(Points2D::Coords coords) override;
		void setSize(Points2D::Size size) override;
		void setSpriteTexture(sf::Texture& texture);
		void setRotation(float);
		void setScale(float, float);
		void draw(sf::RenderWindow& renderWindow);
		void update();
		ImageGUIelement();
	};

	class Label : public ShapeGUIelement
	{
	protected:
		sf::Text text;
		void alignText();
	public:
		void setCoords(Points2D::Coords coords) override;
		void setSize(Points2D::Size size) override;
		void setText(sf::String text);
		void setCharSize(unsigned int charSize);
		bool setFont(sf::Font* font);
		void draw(sf::RenderWindow& renderWindow) override;
		virtual bool userInput(sf::Event& event) override;
		Label();
		virtual ~Label();
	};

	class Button : public Label
	{
	protected:
		sf::Color mouseOverColor = fillColor;
		sf::Color mousePressedColor = fillColor;
	public:
		void setInteractiveColors(sf::Color mouseOverColor, sf::Color mousePressedColor);
		bool userInput(sf::Event& event) override;
		Button();
	};

	class InputBox : public Label
	{
	private:
		uint16_t _maxTextLength = 0;
		std::wstring _textString = L"";
		sf::Clock _timer;
		bool addUnderScore = false;

	private:
		void removeUnderscore(bool updateScreenText);
		void blinkUnderscore();

	public:
		std::wstring _captionString = L"";
		bool (*filterFunction)(wchar_t symbol) = nullptr;
		WindowInputHandlers::KeyboardInputHandler* keyboard = nullptr;

	public:
		InputBox();
		void setMaxTextLength(uint16_t maxTextLength);
		void update() override;
		bool userInput(sf::Event& event) override;
		const std::wstring& getText() const;
	};

	struct MenuElement
	{
		GUIelement* guiElement = nullptr;
		ElementID elementID = -1;
		TabIndex tabIndex = -1;
	};

	class Menu
	{
	private:
		std::vector<MenuElement> menuElements;
		sf::Font* font = nullptr;
		std::vector<MenuElement>::iterator checkIfElementExists(ElementID elementID);
		GUIelement* allocMemory(ElementType elementType, ElementID elementID);

	public:
		sf::RenderWindow* renderWindow = nullptr;
		Menu();
		bool loadFont(sf::Font* font);
		bool addLabel(sf::String text, Points2D::Coords coords, Points2D::Size size, ElementID elementID);
		bool addButton(sf::String text, Points2D::Coords coords, Points2D::Size size, ElementID elementID);
		bool addInputBox(Points2D::Coords coords, Points2D::Size size, ElementID elementID, uint16_t maxStrLen, std::wstring startText);
		bool addImageBox(Points2D::Coords coords, sf::Texture& texture, ElementID elementID);
		bool removeElement(ElementID elementID);
		void alignElementsCenter(sf::Vector2u windowSize);
		ElementID userInput(sf::Event& event);
		void update();
		void draw();
		GUIelement* getElementByID(ElementID elementID);
		~Menu();

	};
}
