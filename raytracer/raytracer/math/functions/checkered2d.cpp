#include "math/functions/checkered2d.h"
#include "math/functions.h"
#include <cmath>

using namespace math;

Function<bool(const Point2D&)> functions::checkered2d(double squareSize)
{
	std::function<bool(const Point2D&)> function = [squareSize](const Point2D& p)
	{
		auto x = std::floor(double(p.x()) / squareSize);
		auto y = std::floor(double(p.y()) / squareSize);

		auto xI = (int)x;
		auto yI = (int)y;

		return std::abs(xI % 2) != std::abs(yI % 2);
	};

	return from_lambda<bool, const Point2D&>(function);
}
