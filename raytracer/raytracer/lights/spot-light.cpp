#include "lights/spot-light.h"

using namespace math;
using namespace imaging;
using namespace raytracer;

namespace
{
	class SpotLight : public lights::_private_::LightSourceImplementation
	{
	public:
		SpotLight(const Point3D& position, const Point3D& direction, const Angle& angle, const Color& color)
			:position(position), direction(direction), theta(angle), color(color) { }

	protected:
		std::vector<LightRay> lightrays_to(const Point3D& point) const override
		{
			// The spot light sends lights to this point P if the angle between LP (where L is the position) and the direction 
			// is less than angle/2. We know the dot product u . v gives us the cosine of the angle between the two vectors u 
			// and v. So we calculate this dot product and call it cos alpha. We call the theta/2 beta. 
			auto u = (point - position).normalized();
			const auto cos_alpha = u.dot((direction-position).normalized());
			const auto cos_beta = cos(theta / 2);

			if (cos_alpha >= cos_beta)
			{
				auto ray = Ray(position, point);
				return std::vector<LightRay> {LightRay(ray, color)};
			}
			return std::vector<LightRay>{};
		}

	private:
		Point3D position;
		Point3D direction;
		Angle theta;
		Color color;
	};
}

LightSource lights::spot(const Point3D& position, const Point3D& direction, const Angle& angle, const Color& color)
{
	return LightSource(std::make_shared<SpotLight>(position, direction, angle, color));
}
