#include "Objects2D.h"

using namespace Objects2D;
using namespace Points2D;

/// Abstract object class ///

Size Object2D::getSize()
{
	return size;
}

Coords Object2D::getCoords()
{
	return coords;
}

Vector Object2D::getSpeed()
{
	return speed;
}

sf::Shape& Object2D::getShape()
{
	return *shape;
}

void Object2D::setCoords(Coords coords)
{
	this->coords = coords;
	this->shape->setPosition(coords.x, coords.y);
}

void Object2D::setSpeed(Vector speed)
{
	this->speed = speed;
}

void Object2D::initObject2D(Coords coords, sf::Color fillColor)
{
	this->coords = coords;
	this->shape->setPosition({ coords.x, coords.y });
	this->shape->setOutlineThickness(BorderThickness);
	this->shape->setOutlineColor(sf::Color::Black);
	this->shape->setFillColor(fillColor);
}

Object2D::Object2D()
{

}

Object2D::~Object2D()
{

}

//////////////////////////////


/// Platform ///

void Platform::setSize(Size size)
{
	this->size = size;
	dynamic_cast<sf::RectangleShape*>(shape)->setSize({ size.x, size.y });
}

Platform::Platform(const Coords coords, const Size platformSize, const sf::Color fillColor)
{
	this->shape = new sf::RectangleShape;
	initObject2D(coords, fillColor);
	this->size = platformSize;
	dynamic_cast<sf::RectangleShape*>(shape)->setSize({platformSize.x, platformSize.y});
}

void Platform::operator= (Platform&& object)
{
	this->coords = object.coords;
	this->speed = object.speed;
	this->size = object.size;
	this->shape = object.shape;
	object.shape = nullptr;
}

Platform::Platform()
{

}

Platform::~Platform()
{
	if (shape)
	{
		delete dynamic_cast<sf::RectangleShape*>(shape);
		shape = nullptr;
	}
}

//////////////


/// Ball ///

void Ball::setSize(Size size)
{
	this->size.x = size.x;
	this->size.y = size.x;
	dynamic_cast<sf::CircleShape*>(shape)->setRadius(size.x);
}

Ball::Ball(const Coords coords, const float ballRadius, const sf::Color fillColor)
{
	this->shape = new sf::CircleShape;
	initObject2D(coords, fillColor);
	this->size.x = 2 * ballRadius;
	this->size.y = 2 * ballRadius;
	dynamic_cast<sf::CircleShape*>(shape)->setRadius(ballRadius);
}

void Ball::operator= (Ball&& object)
{
	this->coords = object.coords;
	this->speed = object.speed;
	this->size = object.size;
	this->shape = object.shape;
	object.shape = nullptr;
}

Ball::Ball()
{

}

Ball::~Ball()
{
	if (shape)
	{
		delete dynamic_cast<sf::CircleShape*>(shape);
		shape = nullptr;
	}
}

////////////
