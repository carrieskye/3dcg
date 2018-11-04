#pragma once

#include "raytracers/ray-tracer.h"
#include "ray-tracer-v5.h"

using namespace imaging;
using namespace math;


namespace raytracer
{
	namespace raytracers
	{
		namespace _private_
		{
			class RayTracerV6 : public RayTracerV5
			{
			public:
				TraceResult trace(const Scene&, const Ray&, const double) const override;
				Color compute_refraction(const Scene&, const MaterialProperties&, const Hit&, const Ray&, const double) const;
			};
		}

		RayTracer v6();
	}
}