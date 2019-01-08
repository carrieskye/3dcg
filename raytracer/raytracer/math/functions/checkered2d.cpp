#include "math/functions/checkered2d.h"
#include "math/functions.h"
#include <cmath>

using namespace math;

Function<bool(const Point2D&)> functions::checkered2d(int squareSize)
{
	std::function<bool(const Point2D&)> function = [squareSize](const Point2D& p)
	{
		auto x = p.x();
		auto y = p.y();

		auto xN = x / squareSize;
		auto yN = y / squareSize;

		xN = std::floor(xN * 2);
		yN = std::floor(yN * 2);

		auto xI = (int)xN;
		auto yI = (int)yN;

		return std::abs(xI % 2) != std::abs(yI % 2);
	};

	return from_lambda<bool, const Point2D&>(function);
}
