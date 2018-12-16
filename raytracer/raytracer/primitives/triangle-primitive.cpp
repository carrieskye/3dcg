#include "triangle-primitive.h"
#include "math/misc.h"

using namespace raytracer;
using namespace primitives;

namespace
{
	class TriangleImplementation : public primitives::_private_::PrimitiveImplementation
	{
	public:
		TriangleImplementation(const Point3D &v1, const Point3D &v2, const Point3D &v3) : v1(v1), v2(v2), v3(v3)
		{
			// compute the normal vector on the plane
			auto v = (v2 - v1).cross(v3 - v1).normalized();
			this->normal = Vector3D(v.x(), v.y(), v.z());
		}


		std::vector<std::shared_ptr<Hit>> find_all_hits(const Ray& ray) const override
		{
			// Allocate vector on stack
			std::vector<std::shared_ptr<Hit>> hits;

			// compute the hit t
			double t = (v1 - ray.origin).dot(normal) / ray.direction.dot(normal);

			// compute the hit position H
			Point3D H = ray.origin + (ray.direction * t);

			// check if H lies to the right if P1 P2
			if (!((v2 - v1).cross(H - v1).dot(normal) >= 0)) return hits;

			// check if H lies to the right if P2 P3
			if (!((v3 - v2).cross(H - v2).dot(normal) >= 0)) return hits;

			// check if H lies to the right if P3 P1
			if (!((v1 - v3).cross(H - v3).dot(normal) >= 0)) return hits;

			auto hit = std::make_shared<Hit>();
			initialize_hit(hit.get(), ray, t);
			hits.push_back(hit);

			return hits;
		}

		bool find_first_positive_hit(const Ray& ray, Hit* output_hit) const override
		{
			//assert(hit != nullptr);

			auto hits = find_all_hits(ray);

			for (auto hit : hits)
			{
				// Find first positive hit
				if (hit->t > 0)
				{
					if (hit->t < output_hit->t)
					{
						// Overwrite hit with new hit
						*output_hit = *hit;
						return true;

					}
					// First positive hit is farther away than already existing hit
					return false;
				}
			}
			// No positive hits were found
			return false;
		}

		Box bounding_box() const override
		{
			auto x_min = std::min({ v1.x(), v2.x(), v3.x() });
			auto y_min = std::min({ v1.y(), v2.y(), v3.y() });
			auto z_min = std::min({ v1.z(), v2.z(), v3.z() });

			auto x_max = std::max({ v1.x(), v2.x(), v3.x() });
			auto y_max = std::max({ v1.y(), v2.y(), v3.y() });
			auto z_max = std::max({ v1.z(), v2.z(), v3.z() });

			return Box(
				interval(x_min, x_max),
				interval(y_min, y_max),
				interval(z_min, z_max)
			);
		}

	private:
		Point3D v1;
		Point3D v2;
		Point3D v3;
		Vector3D normal;


		void initialize_hit(Hit* hit, const Ray& ray, double t) const
		{
			// Update Hit object


			hit->t = t;
			hit->position = ray.at(t);
			hit->local_position.xyz = hit->position;
			hit->local_position.uv = Point2D(
				hit->position.x(),
				hit->position.y());
			hit->normal = normal;
		}
	};
}

Primitive primitives::triangle(const Point3D &v1, const Point3D &v2, const Point3D &v3)
{
	return Primitive(std::make_shared<TriangleImplementation>(v1, v2, v3));
}
