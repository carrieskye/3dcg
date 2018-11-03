#pragma once

#include "math/point.h"
#include "math/point.h"
#include "math/function.h"


namespace math
{
    namespace functions
    {
        typedef Function<double(const double&)> Noise1D;
        typedef Function<double(const Point2D&)> Noise2D;
        typedef Function<double(const Point3D&)> Noise3D;
    }
}