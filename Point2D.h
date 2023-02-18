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
	} Coords, Vector, Size;

}
