#include "Physics2D.h"

using namespace Physics2D;
using namespace Points2D;

Physics::Physics()//(float* timer)
{
	//this->timer = timer;
}

Physics::~Physics()
{
	//this->timer = nullptr;
}

BORDER Physics::resolveWindowFrameToObjectCollisions(Objects2D::Object2D& object, Size windowSize)
{
    BORDER collisionBorder = BORDER_NONE;
	Coords objectCoords = object.getCoords();
	Size objectSize = object.getSize();
	Vector objectSpeed = object.getSpeed();

	bool isLeftWallHit = (objectCoords.x - BorderThickness < 0);
	bool isRightWallHit = (objectCoords.x + objectSize.x + BorderThickness >= windowSize.x);
	bool isBottomWallHit = objectCoords.y + objectSize.y + BorderThickness >= windowSize.y;
	bool isTopWallHit = objectCoords.y - BorderThickness < 0;

	if (isLeftWallHit || isRightWallHit)
	{
        isLeftWallHit
            ? (objectCoords.x = 0 + BorderThickness, collisionBorder |= BORDER_LEFT)
            : (objectCoords.x = windowSize.x - BorderThickness - 1 - objectSize.x, collisionBorder |= BORDER_RIGHT);
        objectSpeed.x *= -1;
	}

	if (isTopWallHit || isBottomWallHit)
	{
        isTopWallHit 
            ? (objectCoords.y = 0 + BorderThickness, collisionBorder |= BORDER_TOP) 
            : (objectCoords.y = windowSize.y - BorderThickness - 1 - objectSize.y, collisionBorder |= BORDER_BOTTOM);
		objectSpeed.y *= -1;
	}

	object.setCoords(objectCoords);
	object.setSpeed(objectSpeed);
    return collisionBorder;
}

void Physics::moveObject(Objects2D::Object2D& object, float objectSpeedMultiplier, float deltaTime)
{
	Coords objectCoords = object.getCoords();
	Vector objectSpeed = object.getSpeed();

	objectCoords.x += objectSpeed.x * objectSpeedMultiplier * deltaTime;
	objectCoords.y += objectSpeed.y * objectSpeedMultiplier * deltaTime;

	object.setCoords(objectCoords);
	//sf::Shape* shape = &(object.getShape());
	//shape->setPosition({objectCoords.x, objectCoords.y});
}

void Physics::resolveBallToBallCollision(Objects2D::Ball& ball, Objects2D::Platform& platform)
{
    Coords platformCoords = platform.getCoords();
    Size platformSize = platform.getSize();
    Vector platformSpeed = platform.getSpeed();

    Coords ballCoords = ball.getCoords();
    Size ballSize = ball.getSize();
    Vector ballSpeed = ball.getSpeed();

    float x, y;
    bool isEntersectionFound = false;

    for (y = ballCoords.y; y <= ballCoords.y + ballSize.y; y += ballSize.y)
    {
        for (x = ballCoords.x; x <= ballCoords.x + ballSize.x; x += ballSize.x)
        {
            if (x <= platformCoords.x + platformSize.x && x >= platformCoords.x)
                if (y <= platformCoords.y + platformSize.y && y >= platformCoords.y)
                {
                    isEntersectionFound = true;
                    break;
                }
        }
        if (isEntersectionFound)
            break;
    }

    if (!isEntersectionFound)
        return;

    Coords platformCenter, ballCenter;
    float dx, dy, d;    // d - hypotenuse

    platformCenter.x = platformCoords.x + platform.getSize().x / 2;
    platformCenter.y = platformCoords.y + platform.getSize().y / 2;

    ballCenter.x = ballCoords.x + ball.getSize().x / 2;
    ballCenter.y = ballCoords.y + ball.getSize().y / 2;

    dx = ballCenter.x - platformCenter.x;
    dy = ballCenter.y - platformCenter.y;
    d = sqrtf(dx * dx + dy * dy);

    float cos = dx / d;
    float sin = dy / d;

    //if (d < platformSize.x + ballSize.x / 2)

    float platformVx = platformSpeed.x * cos - platformSpeed.y * sin;
    float platformVy = platformSpeed.x * sin + platformSpeed.y * cos;

    float ballVx = ballSpeed.x * cos - ballSpeed.y * sin;
    float ballVy = ballSpeed.x * sin + ballSpeed.y * cos;

    //ballVx = platformVx - ballVx;
    ballVx *= -1;
    ballVy *= -1;

    ballSpeed.x = ballVx * cos + ballVy * sin;
    ballSpeed.y = ballVx * sin - ballVy * cos;

    ball.setSpeed(ballSpeed);
}

void Physics::resolveBallToPlatformCollisions(Objects2D::Ball& ball, Objects2D::Platform& platform)
{
    Coords platformCoords = platform.getCoords();
    Size platformSize = platform.getSize();
    Vector platformSpeed = platform.getSpeed();

    Coords ballCoords = ball.getCoords();
    Size ballSize = ball.getSize();
    Vector ballSpeed = ball.getSpeed();
    Size ballRadius = { ballSize.x / 2, ballSize.x / 2 };

    Coords ballCenter = ballCoords + ballRadius;
    Coords closestPoint;

    bool isCollisionDetected = false;
    bool isHorizontalBarCollisionDetected = false;
    bool isVerticalBarCollisionDetected = false;
    
    if (ballCenter.x < platformCoords.x)
    {
        closestPoint.x = platformCoords.x;
    }
    else if (ballCenter.x > platformCoords.x + platformSize.x)
    {
        closestPoint.x = platformCoords.x + platformSize.x;
    }
    else
    {
        closestPoint.x = ballCenter.x;
    }

    if (ballCenter.y < platformCoords.y)
    {
        closestPoint.y = platformCoords.y;
    }
    else if (ballCenter.y > platformCoords.y + platformSize.y)
    {
        closestPoint.y = platformCoords.y + platformSize.y;
    }
    else
    {
        closestPoint.y = ballCenter.y;
    }

    float distanceBetweenClosestPointAndBall = 0;
    float dx, dy;

    dx = fabsf(ballCenter.x - closestPoint.x);
    dy = fabsf(ballCenter.y - closestPoint.y);
    distanceBetweenClosestPointAndBall = sqrtf(dx * dx + dy * dy);

    if (ballRadius.x + 1 >= distanceBetweenClosestPointAndBall)
        isCollisionDetected = true;

    if (isCollisionDetected)
    {
        float ballPlatformMergeX = ballRadius.x + BorderThickness - dx;
        float ballPlatformMergeY = ballRadius.y + BorderThickness - dy;

        if (ballCenter.x < platformCoords.x)
        {
            ballCoords.x -= ballPlatformMergeX;
        }
        else if (ballCenter.x > platformCoords.x + platformSize.x)
        {
            ballCoords.x += ballPlatformMergeX;
        }

        if (ballCenter.y < platformCoords.y)
        {
            ballCoords.y -= ballPlatformMergeY;
        }
        else if (ballCenter.y > platformCoords.y + platformSize.y)
        {
            ballCoords.y += ballPlatformMergeY;
        }

        ball.setCoords(ballCoords);

        if (closestPoint.x == platformCoords.x || closestPoint.x == platformCoords.x + platformSize.x)
            isVerticalBarCollisionDetected = true;

        if (closestPoint.y == platformCoords.y || closestPoint.y == platformCoords.y + platformSize.y)
            isHorizontalBarCollisionDetected = true;
    }

    if (isVerticalBarCollisionDetected)
    {
        if (platformSpeed.y)
        {
            float normalizedVerticalSpeed = platformSpeed.y / fabsf(platformSpeed.y);

            if (ballSpeed.y / fabsf(ballSpeed.y) != normalizedVerticalSpeed)
            {
                float sumOfSquaredVectorSides = ballSpeed.x * ballSpeed.x + ballSpeed.y * ballSpeed.y;
                if (ballSpeed.y > 0.5 || ballSpeed.y < -0.5)
                    ballSpeed.y *= 0.5;
                else
                    ballSpeed.y *= 1.8;
                ballSpeed.x = ballSpeed.x / fabsf(ballSpeed.x) * sqrtf(sumOfSquaredVectorSides - ballSpeed.y * ballSpeed.y);
            }
        }
        ballSpeed.x *= -1;
    }

    if (isHorizontalBarCollisionDetected)
    {
        if (platformSpeed.y)
        {
            float normalizedVerticalSpeed = platformSpeed.y / fabsf(platformSpeed.y);

            if (ballSpeed.y > 0)
                ballSpeed.y *= normalizedVerticalSpeed;
            else if (ballSpeed.y < 0)
                ballSpeed.y *= -1 * normalizedVerticalSpeed;

            if (ballSpeed.y > -0.5 && ballSpeed.y < 0.5)
            {
                float sumOfSquaredVectorSides = ballSpeed.x * ballSpeed.x + ballSpeed.y * ballSpeed.y;
                ballSpeed.y *= 1.8;
                ballSpeed.x = ballSpeed.x / fabsf(ballSpeed.x) * sqrtf(sumOfSquaredVectorSides - ballSpeed.y * ballSpeed.y);
            }
        }
        else
            ballSpeed.y *= -1;
    }

    ball.setSpeed(ballSpeed);
}

void Physics::randomizeBallAngle(Objects2D::Ball& ball)
{
    float ballAngle;
    do
    {
        ballAngle = (rand() % 360) * M_PI / 180.f;
        float horizontalSpeed = cosf(ballAngle);
        float verticalSpeed = sinf(ballAngle);
        ball.setSpeed({ horizontalSpeed, verticalSpeed });
    } while (fabsf(cosf(ballAngle)) < 0.8);
}
