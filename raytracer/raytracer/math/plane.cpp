#include "math/plane.h"
#include <assert.h>

using namespace math;


Plane::Plane(const Point3D& p, const Vector3D& n)
    : point(p), normal(n)
{
    assert(n.is_unit());
}

Plane Plane::from_point_and_normal(const Point3D& p, const Vector3D& n)
{
    assert(n.is_unit());

    return Plane{ p, n };
}

double math::distance(const Plane& plane, const Point3D& point)
{
    assert(plane.normal.is_unit());

    return (point - plane.point).dot(plane.normal);
}

bool math::lies_in_front_of(const Plane& plane, const Point3D& point)
{
    return math::distance(plane, point) > 0;
}

bool math::lies_behind(const Plane& plane, const math::Point3D& point)
{
    return math::distance(plane, point) < 0;
}

bool math::lies_on(const Plane& plane, const math::Point3D& point, double epsilon)
{
    return math::distance(plane, point) == approx(0.0, epsilon);
}
