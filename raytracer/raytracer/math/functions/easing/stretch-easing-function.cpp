#include "math/functions/easing/stretch-easing-function.h"
#include "math/functions.h"

using namespace math;
using namespace functions;


EasingFunction easing::stretch_in_time(EasingFunction function, const Interval<double>& x_range)
{
    return (identity<double>() - x_range.lower) >> (identity<double>() / x_range.size()) >> function;
}

EasingFunction easing::stretch_in_space(EasingFunction function, const Interval<double>& y_range)
{
    return function * y_range.size() + y_range.lower;
}

EasingFunction easing::stretch(EasingFunction function, const Interval<double>& x_range, const Interval<double>& y_range)
{
    return stretch_in_time(stretch_in_space(function, y_range), x_range);
}
