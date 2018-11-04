#pragma once

#include "raytracers/ray-tracer.h"
#include "ray-tracer-v4.h"

using namespace imaging;
using namespace math;


namespace raytracer
{
	namespace raytracers
	{
		namespace _private_
		{
			class RayTracerV5 : public RayTracerV4
			{
			public:
				TraceResult trace(const Scene&, const Ray&) const override;
				virtual TraceResult trace(const Scene&, const Ray&, const double) const;
				Color compute_reflection(const Scene&, const MaterialProperties&, const Hit&, const Ray&, const double) const;
			};
		}

		RayTracer v5();
	}
}