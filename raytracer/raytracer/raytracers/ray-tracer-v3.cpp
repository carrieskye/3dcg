#include "ray-tracer-v3.h"

using namespace imaging;
using namespace math;
using namespace raytracer;

Color raytracers::_private_::RayTracerV3::process_light_ray(const Scene& scene, const MaterialProperties& properties, const Hit& hit, const Ray& ray, const LightRay& light_ray) const
{
	const auto result = compute_specular(properties, hit, ray, light_ray);
	return result;
}

Color raytracers::_private_::RayTracerV3::compute_specular(const MaterialProperties& properties, const Hit& hit, const Ray& ray, const LightRay& light_ray) const
{
	auto l = light_ray.ray.origin;
	auto p = hit.position;
	auto e = ray.origin;

	auto i = (p - l).normalized();
	auto r = i - 2 * i.dot(hit.normal) * hit.normal;
	auto v = (e - p).normalized();

	auto cos_alpha = v.dot(r);

	if (cos_alpha > 0) 
		return  light_ray.color * properties.specular * pow(cos_alpha, properties.specular_exponent);

	return colors::black();
}

RayTracer raytracers::v3()
{
	return RayTracer(std::make_shared<_private_::RayTracerV3>());
}