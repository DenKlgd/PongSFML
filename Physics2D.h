#pragma once

#ifndef _USE_MATH_DEFINES 
	#define _USE_MATH_DEFINES
#endif

#define BORDER_NONE 0b00000000
#define BORDER_LEFT 0b00000001
#define BORDER_RIGHT 0b00000010
#define BORDER_TOP 0b00000100
#define BORDER_BOTTOM 0b00001000
#define BORDER unsigned char

#include <cmath>
#include "Objects2D.h"

namespace Physics2D
{
	class Physics
	{
	private:
		//float* timer = nullptr;

	public:
		Physics();//(float* timer);
		~Physics();
		BORDER resolveWindowFrameToObjectCollisions(Objects2D::Object2D& object, Points2D::Size windowSize);
		void resolveBallToBallCollision(Objects2D::Ball& ball, Objects2D::Platform& platform);
		void resolveBallToPlatformCollisions(Objects2D::Ball& ball, Objects2D::Platform& platform);
		void moveObject(Objects2D::Object2D& object, float objectSpeedMultiplier, float deltaTime);
		void randomizeBallAngle(Objects2D::Ball& ball);

	};
}