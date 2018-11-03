#pragma once

#include "math/function.h"


namespace math
{
    namespace functions
    {
        /// <summary>
        /// Define EasingFunction as a synonym for math::Function&lt;double(double)&gt;.
        /// </summary>
        typedef Function<double(double)> EasingFunction;
    }
}