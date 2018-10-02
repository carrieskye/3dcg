#pragma once

#include "math/function.h"
#include "math/point.h"

namespace math
{
	namespace functions
	{
		math::Function<bool(const Point2D&)> checkered2d(int size = 1);
	}
}