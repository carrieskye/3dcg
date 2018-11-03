#pragma once

#include "math/functions/easing-function.h"
#include "math/interval.h"


namespace math
{
    namespace functions
    {
        namespace easing
        {
            EasingFunction stretch_in_time(EasingFunction function, const Interval<double>& x_range);
            EasingFunction stretch_in_space(EasingFunction function, const Interval<double>& y_range);
            EasingFunction stretch(EasingFunction function, const Interval<double>& x_range, const Interval<double>& y_range);
        }
    }
}