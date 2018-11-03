#pragma once

#include "raytracers/ray-tracer.h"
#include "ray-tracer-v2.h"

using namespace imaging;
using namespace math;


namespace raytracer
{
	namespace raytracers
	{
		namespace _private_
		{
			class RayTracerV3 : public RayTracerV2
			{
			public:
				Color process_light_ray(const Scene&, const MaterialProperties&, const Hit&, const Ray&, const LightRay&) const override;
				virtual Color compute_specular(const MaterialProperties&, const Hit&, const Ray&, const LightRay&) const;
			};
		}

		RayTracer v3();
	}
}