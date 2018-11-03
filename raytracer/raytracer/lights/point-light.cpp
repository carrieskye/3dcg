#include "lights/point-light.h"

using namespace math;
using namespace raytracer;
using namespace imaging;


lights::_private_::PointLightImplementation::PointLightImplementation(const Point3D& position)
    : m_position(position) { }

std::vector<LightRay> lights::_private_::PointLightImplementation::lightrays_to(const Point3D& p) const
{
    auto result = this->cast_lightray_to(p);

    // Only return non-black light rays
    if (result.color != colors::black())
    {
        return std::vector<LightRay> { result };
    }
    else
    {
        return std::vector<LightRay>();
    }
}
