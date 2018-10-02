#include "math/functions/checkered2d.h"
#include "math/functions.h"
#include <cmath>

using namespace math;

Function<bool(const Point2D&)> math::functions::checkered2d(double thickness)
{
	std::function<bool(const Point2D&)> function = [thickness](const Point2D& p)
	{
		auto x = p.x();
		auto y = p.y();

		return (std::abs(x - round(x)) < thickness) || (std::abs(y - round(y)) < thickness);
	};

	return from_lambda<bool, const Point2D&>(function);
}
