#pragma once

#include "math/function.h"
#include "math/point.h"

namespace math
{
	namespace functions
	{
		Function<bool(const Point2D&)> checkered2d(double size = 1);
	}
}