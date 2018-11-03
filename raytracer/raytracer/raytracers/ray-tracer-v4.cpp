#include "ray-tracer-v4.h"

using namespace imaging;
using namespace math;
using namespace raytracer;

Color raytracers::_private_::RayTracerV4::process_light_ray(const Scene& scene, const MaterialProperties& properties, const Hit& hit, const Ray& ray, const LightRay& light_ray) const
{
	Color result = colors::black();

	Hit first_positive_hit;

	if (scene.root->find_first_positive_hit(light_ray.ray, &first_positive_hit))
	{
		auto t = first_positive_hit.t;

		if (0 <= t && t < 1)
		{
			return result;
		}
	}

	result += RayTracerV3::process_light_ray(scene, properties, hit, ray, light_ray);
	return result;
}

RayTracer raytracers::v4()
{
	return RayTracer(std::make_shared<_private_::RayTracerV4>());
}