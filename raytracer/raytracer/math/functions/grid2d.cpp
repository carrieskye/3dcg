#include "math/functions/grid2d.h"
#include "math/functions.h"
#include <cmath>

using namespace math;


Function<bool(const Point2D&)> functions::grid2d(double thickness)
{
	std::function<bool(const Point2D&)> function = [thickness](const Point2D& p)
	{
		auto x = p.x();
		auto y = p.y();

		return (std::abs(x - round(x)) < thickness / 2) || (std::abs(y - round(y)) < thickness / 2);
	};

	return from_lambda<bool, const Point2D&>(function);
}
