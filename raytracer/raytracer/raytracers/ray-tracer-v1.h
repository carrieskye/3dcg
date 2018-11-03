#pragma once

#include "raytracers/ray-tracer.h"

using namespace imaging;
using namespace math;

namespace raytracer
{
	namespace raytracers
	{
		namespace _private_
		{
			class RayTracerV1 : public RayTracerImplementation
			{
			public:
				TraceResult trace(const Scene&, const Ray&) const override;
				Color compute_ambient(const MaterialProperties&) const;
			};
		}

		RayTracer v1();
	}
}