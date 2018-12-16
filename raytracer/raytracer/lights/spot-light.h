#pragma once

#include "lights/point-light.h"
#include "imaging/color.h"
#include "math/point.h"
#include <memory>

using namespace math;

namespace raytracer
{
	namespace lights
	{
		LightSource spot(const Point3D& position, const Point3D& direction, const Angle& angle, const imaging::Color& color);
	}
}