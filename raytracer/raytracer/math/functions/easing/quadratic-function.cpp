#include "quadratic-function.h"
#include <cassert>

using namespace math::functions;

// http://www.gizma.com/easing
EasingFunction easing::quadratic_in()
{
	std::function<double(double)> lambda = [](double t) {
		assert(interval(0.0, 1.0).contains(t));
		return t * t;
	};

	return from_lambda(lambda);
}

EasingFunction easing::quadratic_out()
{
	std::function<double(double)> lambda = [](double t) {
		assert(interval(0.0, 1.0).contains(t));
		return -1 * t * (t - 2);
	};

	return from_lambda(lambda);
}

EasingFunction easing::quadratic_inout()
{
	std::function<double(double)> lambda = [](double t) {
		assert(interval(0.0, 1.0).contains(t));
		t /= 0.5;
		if (t < 1) return 0.5 * t * t;
		t--;
		return -0.5 * (t * (t - 2) - 1);
	};

	return from_lambda(lambda);
}