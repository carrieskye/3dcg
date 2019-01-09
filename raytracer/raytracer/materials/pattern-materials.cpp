#include "materials/material.h"
#include "materials/composition-material.h"
#include "materials/pattern-materials.h"
#include "math/function.h"
#include "math/functions.h"
#include "math/functions/patterns.h"
#include "math/functions/noise.h"
#include "math/functions/bool-mapper.h"
#include "imaging/color-mapper.h"

using namespace math;
using namespace raytracer;
using namespace materials;
using namespace imaging;


Material materials::pattern2d(Function<bool(const Point2D&)> pattern, Material m1, Material m2)
{
    auto bool_mapper = functions::bool_mapper(m1, m2);

    return composite(pattern >> bool_mapper);
}

Material materials::horizontal_lines(double thickness, Material m1, Material m2)
{
    return pattern2d(functions::horizontal_lines(thickness), m1, m2);
}

Material materials::vertical_lines(double thickness, Material m1, Material m2)
{
	return pattern2d(functions::vertical_lines(thickness), m1, m2);
}

Material materials::grid2d(double thickness, Material m1, Material m2)
{
	return pattern2d(functions::grid2d(thickness), m1, m2);
}

Material materials::checkered2d(Material m1, Material m2)
{
	return pattern2d(functions::checkered2d(0.5), m1, m2);
}
