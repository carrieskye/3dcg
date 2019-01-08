﻿#include "cubic-function.h"
#include <cassert>

using namespace math::functions;

// http://www.gizma.com/easing
EasingFunction easing::cubic_in()
{
	std::function<double(double)> lambda = [](double t) {
		assert(interval(0.0, 1.0).contains(t));
		return t * t * t;
	};

	return from_lambda(lambda);
}

EasingFunction easing::cubic_out()
{
	std::function<double(double)> lambda = [](double t) {
		assert(interval(0.0, 1.0).contains(t));
		t--;
		return t * t * t + 1;
	};

	return from_lambda(lambda);
}

EasingFunction easing::cubic_inout()
{
	std::function<double(double)> lambda = [](double t) {
		assert(interval(0.0, 1.0).contains(t));
		t /= 0.5;
		if (t < 1) return 0.5 * t * t * t;
		t -= 2;
		return 0.5 * (t * t * t + 2);
	};

	return from_lambda(lambda);
}