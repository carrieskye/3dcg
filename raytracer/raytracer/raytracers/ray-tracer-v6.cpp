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

Color raytracers::_private_::RayTracerV6::compute_refraction(const Scene& scene, const MaterialProperties& enter_properties, const Hit& hit, const Ray& ray, const double weight) const
{
	const auto result = colors::black();

	if (enter_properties.transparency > 0)
	{
		const double n1 = 1;
		const auto n2 = enter_properties.refractive_index;

		auto i = (hit.position - ray.origin).normalized();
		const auto n = hit.normal;

		auto ox = (n1 / n2) * (i - i.dot(n)* n);
		const auto oy = (-1 * sqrt(1 - ox.dot(ox))) * n;
		const auto o = ox + oy;

		if (1 - ox.dot(ox) < 0)
		{
			return result;
		}

		const auto enter_vector_moved_slightly = hit.position + o * 0.0000001;
		const auto enter_ray = Ray(enter_vector_moved_slightly, o);

		Hit exit_hit;

		if(scene.root->find_first_positive_hit(enter_ray, &exit_hit))
		{
			auto exit_properties = exit_hit.material->at(exit_hit.local_position);
			if (!(exit_properties.transparency > 0)) {
				return trace(scene, enter_ray, weight * enter_properties.transparency).color * enter_properties.transparency;
			}
			auto exit_i = enter_ray.direction.normalized();
			auto exit_n = exit_hit.normal;
			auto exit_ox = (n2 / n1) * (exit_i - exit_i.dot(exit_n)* exit_n);
			
			const auto exit_oy = (-1 * sqrt(1 - exit_ox.dot(exit_ox))) * exit_n;
			const auto exit_o = exit_ox + exit_oy;

			if (1 - exit_ox.dot(exit_ox) < 0)
			{
				return result;
			}

			const auto exit_vector_moved_slightly = exit_hit.position + exit_o * 0.0000001;
			const auto exit_ray = Ray(exit_vector_moved_slightly, exit_o);
			return trace(scene, exit_ray, weight * exit_properties.transparency).color * exit_properties.transparency;
		}
	}

	return result;
}

raytracer::RayTracer raytracers::v6()
{
	return RayTracer(std::make_shared<_private_::RayTracerV6>());
}
