#include "cone-primitive.h"
#include "math/interval.h"
#include "math/quadratic_equation.h"
#include "util/misc.h"

using namespace raytracer;
using namespace primitives;
using namespace math;


namespace
{
	/// <summary>
	/// Superclass for planes. Contains common logic.
	/// </summary>
	class ConeImplementation : public primitives::_private_::PrimitiveImplementation
	{
	protected:
		const Vector3D m_normal;

		/// <summary>
		/// Constructor.
		/// </summary>
		/// <param name="normal">
		/// Normal vector on plane. Needs to have unit length.
		/// </param>
		ConeImplementation(const Vector3D& normal) : m_normal(normal)
		{
			assert(normal.is_unit());
		}

		virtual void initialize_hit(Hit* hit, const Ray& ray, double t) const = 0;

	public:
		std::vector<std::shared_ptr<Hit>> find_all_hits(const Ray& ray) const override
		{
			// See sphere formulae
			double a = ray.direction.dot(ray.direction);
			double b = 2 * ray.direction.dot(ray.origin - Point3D());
			double c = (ray.origin - Point3D()).norm_sqr() - 1;

			QuadraticEquation qeq(a, b, c);

			// If the ray hits the sphere, the quadratic equation will have solutions
			if (qeq.has_solutions())
			{
				// Quadratic equation has solutions: there are two intersections
				double t1 = qeq.x1();
				double t2 = qeq.x2();

				// We want t1 &lt; t2, swap them if necessary
				sort(t1, t2);

				// Sanity check (only performed in debug builds)
				assert(t1 <= t2);

				// Allocate vector on stack
				std::vector<std::shared_ptr<Hit>> hits;

				// Allocate two Hit objects on heap and store address in shared pointers
				auto hit1 = std::make_shared<Hit>();
				auto hit2 = std::make_shared<Hit>();

				// Initialize both hits
				initialize_hit(hit1.get(), ray, t1);
				initialize_hit(hit2.get(), ray, t2);

				// Put hits in vector
				hits.push_back(hit1);
				hits.push_back(hit2);

				// Return hit list
				return hits;
			}
			else
			{
				// No intersections to be found
				return std::vector<std::shared_ptr<Hit>>();
			}
		}

		bool find_first_positive_hit(const Ray& ray, Hit* hit)  const override
		{
			double a = ray.direction.dot(ray.direction);
			double b = 2 * ray.direction.dot(ray.origin - Point3D(0, 0, 0));
			double c = (ray.origin - Point3D()).norm_sqr() - 1;

			QuadraticEquation qeq(a, b, c);

			if (qeq.has_solutions())
			{
				double t1 = qeq.x1();
				double t2 = qeq.x2();
				double t;

				// Find smallest positive t-value 
				if (smallest_greater_than_zero(t1, t2, &t))
				{
					// Check that our new t is better than the pre-existing t
					if (t < hit->t)
					{
						initialize_hit(hit, ray, t);

						return true;
					}
				}
			}

			return false;
		}
	};

	class ConeAlongXImplementation : public ConeImplementation {
	public:
		ConeAlongXImplementation() : ConeImplementation(Vector3D(1, 0, 0))
		{
			// NOP
		}

		Box bounding_box() const override
		{
			auto range = interval(-1.0, 1.0);

			return Box(range, range, range);
		}

	protected:
		void initialize_hit(Hit* hit, const Ray& ray, double t) const override
		{
			hit->t = t;
			hit->position = ray.at(hit->t);
			hit->local_position.xyz = hit->position;
			hit->local_position.uv = Point2D(hit->position.x(), hit->position.y());
			hit->normal = ray.origin.z() > 0 ? m_normal : -m_normal;
		}
	};

	class ConeAlongYImplementation : public ConeImplementation {
	public:
			ConeAlongYImplementation() : ConeImplementation(Vector3D(0, 1, 0))

		{
			// NOP
		}

		Box bounding_box() const override
		{
			return Box(Interval<double>::infinite(), Interval<double>::infinite(), interval(-0.01, 0.01));
		}

	protected:
		void initialize_hit(Hit* hit, const Ray& ray, double t) const override
		{
			hit->t = t;
			hit->position = ray.at(hit->t);
			hit->local_position.xyz = hit->position;
			hit->local_position.uv = Point2D(hit->position.y(), hit->position.z());
			hit->normal = ray.origin.x() > 0 ? m_normal : -m_normal;
		}
	};

	class ConeAlongZImplementation : public ConeImplementation {
	public:
		ConeAlongZImplementation() : ConeImplementation(Vector3D(0, 0, 1))
		{
			// NOP
		}

		Box bounding_box() const override
		{
			return Box(Interval<double>::infinite(), Interval<double>::infinite(), interval(-0.01, 0.01));
		}

	protected:
		void initialize_hit(Hit* hit, const Ray& ray, double t) const override
		{
			hit->t = t;
			hit->position = ray.at(hit->t);
			hit->local_position.xyz = hit->position;
			hit->local_position.uv = Point2D(hit->position.x(), hit->position.z());
			hit->normal = ray.origin.y() > 0 ? m_normal : -m_normal;
		}
	};
}

Primitive primitives::cone_along_x()
{
	return Primitive(std::make_shared<ConeAlongXImplementation>());
}

Primitive primitives::cone_along_y()
{
	return Primitive(std::make_shared<ConeAlongYImplementation>());
}

Primitive primitives::cone_along_z()
{
	return Primitive(std::make_shared<ConeAlongZImplementation>());
}