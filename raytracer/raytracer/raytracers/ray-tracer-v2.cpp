#include "raytracers/ray-tracer-v1.h"
#include "ray-tracer-v2.h"

using namespace imaging;
using namespace math;
using namespace raytracer;


TraceResult raytracer::raytracers::_private_::RayTracerV2::trace(const Scene& scene, const Ray& ray) const
{
	Hit hit;

	// Ask the scene for the first positive hit, i.e. the closest hit in front of the eye
	// If there's a hit, find_first_positive_hit returns true and updates the hit object with information about the hit
	if (scene.root->find_first_positive_hit(ray, &hit))
	{
		// There's been a hit
		// Fill in TraceResult object with information about the trace

		// This ray tracer always returns white in case of a hit
		Color result = compute_ambient(hit.material->at(hit.local_position));
		result += process_lights(scene, hit.material->at(hit.local_position), hit, ray);

		// The hit object contains the group id, just copy it (group ids are important for edge detection)
		unsigned group_id = hit.group_id;

		// The t-value indicates where the ray/scene intersection took place.
		// You can use ray.at(t) to find the xyz-coordinates in space.
		double t = hit.t;

		// Group all this data into a TraceResult object.
		return TraceResult(result, group_id, ray, t);
	}
	else {
		// The ray missed all objects in the scene
		// Return a TraceResult object representing "no hit found"
		// which is basically the same as returning black
		return TraceResult::no_hit(ray);
	}
}

Color raytracer::raytracers::_private_::RayTracerV2::process_lights(const Scene& scene, const MaterialProperties& properties, const Hit& hit, const math::Ray& ray) const
{
	Color result = colors::black();
	for (auto light_source : scene.light_sources)
	{
		result += process_light_source(scene, properties, hit, ray, light_source);
	}
	return result;
}

Color raytracer::raytracers::_private_::RayTracerV2::process_light_source(const Scene& scene, const MaterialProperties& properties, const Hit& hit, const Ray& ray, LightSource light_source) const
{
	Color result = colors::black();
	for (auto light_ray : light_source->lightrays_to(hit.position))
	{
		result += process_light_ray(scene, properties, hit, ray, light_ray);
	}
	return result;
}

Color raytracer::raytracers::_private_::RayTracerV2::process_light_ray(const Scene& scene, const MaterialProperties& properties, const Hit& hit, const Ray& ray, const LightRay& light_ray) const
{
	Color result = colors::black();
	result = compute_diffuse(properties, hit, ray, light_ray);
	return result;
}

Color raytracer::raytracers::_private_::RayTracerV2::compute_diffuse(const MaterialProperties& properties, const Hit& hit, const Ray& ray, const LightRay& light_ray) const
{
	auto l = light_ray.ray.origin;
	auto p = hit.position;

	auto cos_alpha = (l - p).normalized().dot(hit.normal);

	if (cos_alpha > 0) return cos_alpha * light_ray.color * properties.diffuse;
	return colors::black();

}

raytracer::RayTracer raytracer::raytracers::v2()
{
	return raytracer::RayTracer(std::make_shared<raytracer::raytracers::_private_::RayTracerV2>());
}