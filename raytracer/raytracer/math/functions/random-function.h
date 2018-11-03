#pragma once

#include "math/function.h"
#include "math/vector.h"
#include "data-structures/position.h"


namespace math
{
    namespace functions
    {
        Function<unsigned(unsigned)> random_function(unsigned);

        namespace random
        {
            Function<double(unsigned)> unsigned_to_double(unsigned seed);
            Function<Vector2D(const Position2D&)> position_to_vector2d(unsigned seed);
            Function<Vector3D(const Position3D&)> position3d_to_vector3d(unsigned seed);
        }
    }
}