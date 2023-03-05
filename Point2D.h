#pragma once

namespace Points2D {

	typedef struct Point2D {
		float x = 0;
		float y = 0;
		Point2D operator+ (Point2D point)
		{
			Point2D newPoint;
			newPoint.x = this->x + point.x;
			newPoint.y = this->y + point.y;
			return newPoint;
		}
		Point2D& operator* (float multiplyer)
		{
			this->x *= multiplyer;
			this->y *= multiplyer;
			return *this;
		}
		Point2D& operator* (Point2D multiplyer)
		{
			this->x *= multiplyer.x;
			this->y *= multiplyer.y;
			return *this;
		}
		Point2D& operator= (Point2D point)
		{
			this->x = point.x;
			this->y = point.y;
			return *this;
		}
		Point2D& operator= (float point[2])
		{
			this->x = point[0];
			this->y = point[1];
			return *this;
		}
		Point2D() = default;
		Point2D(float x, float y)
		{
			this->x = x;
			this->y = y;
		}
	} Coords, Vector, Size;
}
