#include "ray-tracer-v5.h"

using namespace imaging;
using namespace math;
using namespace raytracer;

raytracer::TraceResult raytracer::raytracers::_private_::RayTracerV5::trace(const Scene& scene, const Ray& ray) const
{
	return trace(scene, ray, 1.0);
}

raytracer::TraceResult raytracer::raytracers::_private_::RayTracerV5::trace(const Scene& scene, const Ray& ray, const double weight) const
{
	Hit hit;

	if (weight > 0.01 && scene.root->find_first_positive_hit(ray, &hit))
	{
		const auto group_id = hit.group_id;
		const auto t = hit.t;
		const auto properties = hit.material->at(hit.local_position);

		Color result = compute_ambient(properties);
		result += process_lights(scene, properties, hit, ray);
		result += compute_reflection(scene, properties, hit, ray, weight);

		return TraceResult(result, group_id, ray, t);
	}
	return TraceResult::no_hit(ray);
}

Color raytracer::raytracers::_private_::RayTracerV5::compute_reflection(const Scene& scene, const MaterialProperties& properties, const Hit& hit, const Ray& ray, const double weight) const
{
	auto result = colors::black();
	if (weight > 0)
	{
		const auto t = hit.normal.normalized();
		const auto reflection_direction = ray.direction.reflect_by(t);
		const auto reflection_origin = ray.at(hit.t) + 0.00000001 * reflection_direction;
		const auto reflection_ray = Ray(reflection_origin, reflection_direction);

		result += (properties.reflectivity * trace(scene, reflection_ray, weight * properties.reflectivity).color);
	}
	return result;
}

raytracer::RayTracer raytracers::v5()
{
	return RayTracer(std::make_shared<_private_::RayTracerV5>());
}