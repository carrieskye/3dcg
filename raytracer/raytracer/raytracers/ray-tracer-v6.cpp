#include "ray-tracer-v6.h"

using namespace imaging;
using namespace math;
using namespace raytracer;

raytracer::TraceResult raytracer::raytracers::_private_::RayTracerV6::trace(const Scene& scene, const Ray& ray, const double weight) const
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
		result += compute_refraction(scene, properties, hit, ray, weight);

		return TraceResult(result, group_id, ray, t);
	}
	return TraceResult::no_hit(ray);
}

Color raytracers::_private_::RayTracerV6::compute_refraction(const Scene& scene, const MaterialProperties& properties, const Hit& hit, const Ray& ray, const double weight) const
{
	const auto result = colors::black();

	if (properties.transparency > 0)
	{
		const double n1 = 1;
		const auto n2 = properties.refractive_index;

		auto i = (hit.position - ray.origin).normalized();
		const auto n = hit.normal;

		auto ox = (n1 / n2) * (i - i.dot(n)* n);
		const auto oy = (-1 * sqrt(1 - ox.dot(ox))) * n;
		const auto o = ox + oy;

		if (1 - ox.dot(ox) < 0)
		{
			return result;
		}

		const auto p1_moved_slightly = hit.position + o * 0.0000001;
		const auto p1_ray = Ray(p1_moved_slightly, o);

		Hit exit_hit;

		if(scene.root->find_first_positive_hit(p1_ray, &exit_hit))
		{
			return trace(scene, p1_ray, weight * properties.transparency).color;
		}
	}

	return result;
}

raytracer::RayTracer raytracers::v6()
{
	return RayTracer(std::make_shared<_private_::RayTracerV6>());
}
