#pragma once

#include "primitives/primitive.h"
#include "math/transformation3d.h"
#include <memory>

using namespace math;


namespace raytracer
{
    namespace primitives
    {
        Primitive transform(const Transformation3D&, Primitive);
        Primitive translate(const Vector3D&, Primitive);        
		Primitive rotate_around_x(Angle, Primitive);
		Primitive rotate_around_y(Angle, Primitive);
		Primitive rotate_around_z(Angle, Primitive);
	}
}