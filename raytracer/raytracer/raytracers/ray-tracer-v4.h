#pragma once

#include "raytracers/ray-tracer.h"
#include "ray-tracer-v3.h"

using namespace imaging;
using namespace math;


namespace raytracer
{
	namespace raytracers
	{
		namespace _private_
		{
			class RayTracerV4 : public RayTracerV3
			{
			public:
				Color process_light_ray(const Scene&, const MaterialProperties&, const Hit&, const Ray&, const LightRay&) const override;
			};
		}

		RayTracer v4();
	}
}