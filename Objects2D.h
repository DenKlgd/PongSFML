#pragma once
#include "SFML/Graphics.hpp"
#include "Point2D.h"

#define BorderThickness 2

namespace Objects2D{

	class Object2D
	{
	protected:
		Points2D::Coords coords;
		Points2D::Vector speed;
		Points2D::Size size;
		sf::Shape* shape = nullptr;
		void initObject2D(Points2D::Coords coords, sf::Color fillColor);

	public:
		Points2D::Size getSize();
		Points2D::Coords getCoords();
		Points2D::Vector getSpeed();
		sf::Shape& getShape();
		void setCoords(Points2D::Coords coords);
		void setSpeed(Points2D::Vector speed);
		virtual void setSize(Points2D::Size size) = 0;
		Object2D();
		virtual ~Object2D();
	};

	class Platform : public Object2D
	{
	private:

	public:
		void setSize(Points2D::Size size) override;
		Platform(Points2D::Coords coords, Points2D::Size platformSize, sf::Color fillColor);
		void Platform::operator= (Platform&& object);
		~Platform();
		Platform();
	};

	class Ball : public Object2D
	{
	private:

	public:
		void setSize(Points2D::Size size) override;
		Ball(const Points2D::Coords coords, const float ballRadius, const sf::Color fillColor);
		void operator= (Ball&& object);
		~Ball();
		Ball();
	};

}