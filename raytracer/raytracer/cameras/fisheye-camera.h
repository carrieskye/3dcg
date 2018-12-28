#pragma once

#include "cameras/displaceable-camera.h"
#include "math/vector.h"
#include "math/point.h"
#include "math/point.h"
#include "math/rectangle3d.h"
#include <memory>

using namespace math;

namespace raytracer
{
	namespace cameras
	{
		namespace _private_
		{
			class FisheyeCamera : public DisplaceableCamera
			{
			public:
				FisheyeCamera(const math::Matrix4x4 transformation, Angle horizontal_angle, Angle vertical_angle);

			protected:
				void enumerate_untransformed_rays(const math::Point2D&, std::function<void(const math::Ray&)>) const;

			private:
				Angle horizontal_angle;
				Angle vertical_angle;
			};
		}

		/// <summary>
		/// Creates a perspective camera.
		/// </summary>
		/// <param name="eye">The camera's location</param>
		/// <param name="look_at">The point the camera is directed at</param>
		/// <param name="up">Specifies how the camera is tilted sideways. (0,1,0) is standard.</param>
		Camera fisheye(
			const Point3D& eye,
			const Point3D& look_at,
			const Vector3D& up,
			Angle horizontal_angle,
			Angle vertical_angle
			);
	}
}