﻿#include "cameras/fisheye-camera.h"
#include "math/rectangle3d.h"
#include <assert.h>
#include "math/interval-mapper.h"
#include "math/transformation-matrices.h"


using namespace raytracer;
using namespace cameras;
using namespace math;


cameras::_private_::FisheyeCamera::FisheyeCamera(const Matrix4x4 transformation, Angle horizontal_angle, Angle vertical_angle)
	: DisplaceableCamera(transformation), horizontal_angle(horizontal_angle), vertical_angle(vertical_angle)
{
	// NOP
}

void cameras::_private_::FisheyeCamera::enumerate_untransformed_rays(const Point2D& point, std::function<void(const Ray&)> callback) const
{
	assert(interval(0.0, 1.0).contains(point.x()));
	assert(interval(0.0, 1.0).contains(point.y()));

	const auto interval_horizontal = interval(-(horizontal_angle.degrees() / 2) - 90, (horizontal_angle.degrees() / 2) - 90);
	const auto interval_vertical = interval(-(vertical_angle.degrees() / 2), vertical_angle.degrees() / 2);

	const auto x_mapper = IntervalMapper<double, double>(interval(0.0, 1.0), interval_horizontal);
	const auto y_mapper = IntervalMapper<double, double>(interval(0.0, 1.0), interval_vertical);

	const auto p = Point3D::spherical(1, Angle::degrees(x_mapper[point.x()]), Angle::degrees(y_mapper[point.y()]));

	callback(Ray(Point3D(0, 0, 0), p));
}

Camera cameras::fisheye(
	const Point3D& eye,
	const Point3D& look_at,
	const Vector3D& up,
	const Angle horizontal_angle,
	const Angle vertical_angle
	)
{
	assert(up.is_unit());

	Matrix4x4 transformation = _private_::create_transformation(eye, look_at, up);

	return Camera(std::make_shared<_private_::FisheyeCamera>(transformation, horizontal_angle, vertical_angle));
}