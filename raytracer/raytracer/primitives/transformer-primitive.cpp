#include "primitives/transformer-primitive.h"
#include "math/misc.h"
#include <algorithm>
#include <assert.h>

using namespace math;
using namespace raytracer;
using namespace primitives;


namespace
{
    class TransformerImplementation : public primitives::_private_::PrimitiveImplementation
    {
    public:
        TransformerImplementation(const Transformation3D& transformer, Primitive transformee)
            : transformer(transformer), transformee(transformee) { }

        bool find_first_positive_hit(const Ray& ray, Hit* hit) const override
        {
            Ray transformed_ray = ray.transform(this->transformer.inverse_transformation_matrix);

            if (this->transformee->find_first_positive_hit(transformed_ray, hit))
            {
                hit->position = this->transformer.transformation_matrix * hit->position;
                hit->normal = (this->transformer.transformation_matrix * hit->normal).normalized();

                return true;
            }
            else
            {
                return false;
            }
        }

        std::vector<std::shared_ptr<Hit>> find_all_hits(const Ray& ray) const override
        {
            Ray transformed_ray = ray.transform(this->transformer.inverse_transformation_matrix);

            auto hits = this->transformee->find_all_hits(transformed_ray);

            for (auto& hit : hits)
            {
                hit->position = this->transformer.transformation_matrix * hit->position;
                hit->normal = (this->transformer.transformation_matrix * hit->normal).normalized();
            }

            return hits;
        }

        Box bounding_box() const override
        {
            auto box = transformee->bounding_box();

            assert(!box.is_infinite());

#define AUX(I, J, K)       Point3D p ## I ## J ## K = this->transformer.transformation_matrix * box.corner<I, J, K>()
            AUX(0, 0, 0);
            AUX(0, 0, 1);
            AUX(0, 1, 0);
            AUX(0, 1, 1);
            AUX(1, 0, 0);
            AUX(1, 0, 1);
            AUX(1, 1, 0);
            AUX(1, 1, 1);
#undef AUX

            double min_x = minimum(p000.x(), p001.x(), p010.x(), p011.x(), p100.x(), p101.x(), p110.x(), p111.x());
            double max_x = maximum(p000.x(), p001.x(), p010.x(), p011.x(), p100.x(), p101.x(), p110.x(), p111.x());

            double min_y = minimum(p000.y(), p001.y(), p010.y(), p011.y(), p100.y(), p101.y(), p110.y(), p111.y());
            double max_y = maximum(p000.y(), p001.y(), p010.y(), p011.y(), p100.y(), p101.y(), p110.y(), p111.y());

            double min_z = minimum(p000.z(), p001.z(), p010.z(), p011.z(), p100.z(), p101.z(), p110.z(), p111.z());
            double max_z = maximum(p000.z(), p001.z(), p010.z(), p011.z(), p100.z(), p101.z(), p110.z(), p111.z());

            Point3D lower_corner(min_x, min_y, min_z);
            Point3D upper_corner(max_x, max_y, max_z);

            return Box::from_raw_corners(lower_corner, upper_corner);
        }

    private:
        Transformation3D transformer;
        Primitive transformee;
    };
}

Primitive primitives::transform(const Transformation3D& transformation, Primitive transformee)
{
    return Primitive(std::make_shared<TransformerImplementation>(transformation, transformee));
}

Primitive primitives::translate(const Vector3D& v, Primitive transformee)
{
    return transform(transformations::translation(v), transformee);
}

Primitive primitives::rotate_around_x(const Angle a, const Primitive transformee)
{
	return transform(transformations::rotate_around_x(a), transformee);
}

Primitive primitives::rotate_around_y(const Angle a, const Primitive transformee)
{
	return transform(transformations::rotate_around_y(a), transformee);
}

Primitive primitives::rotate_around_z(const Angle a, const Primitive transformee)
{
	return transform(transformations::rotate_around_z(a), transformee);
}

Primitive primitives::scale(const double sx, const double sy, const double sz, Primitive transformee)
{
	return transform(transformations::scale(sy, sy, sz), transformee);
}