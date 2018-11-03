#include "math/functions/checkered2d.h"
#include "math/functions.h"
#include <cmath>

using namespace math;

Function<bool(const Point2D&)> functions::checkered2d(int squareSize)
{
	std::function<bool(const Point2D&)> function = [squareSize](const Point2D& p)
	{
		auto size = squareSize;
		if (squareSize < 1)
			size = 1;
		auto double_size = size * 2;

		auto x = p.x();
		auto y = p.y();
		auto x_scaled = std::abs(static_cast<int>(x));
		auto y_scaled = std::abs(static_cast<int>(y));

		if (x > 0 && y > 0 || x < 0 && y < 0)
			return (x_scaled % double_size >= size && y_scaled % double_size >= size
				|| x_scaled % double_size < size && y_scaled % double_size < size);

		return (x_scaled % double_size < size && y_scaled % double_size >= size
			|| x_scaled % double_size >= size && y_scaled % double_size < size);
	};

	return from_lambda<bool, const Point2D&>(function);
}
