#include "math/transformation3d.h"
#include "math/transformation-matrices.h"

using namespace math;


Transformation3D transformations::translation(const Vector3D& v)
{
    Matrix4x4 tm = transformation_matrices::translation(v);
    Matrix4x4 itm = transformation_matrices::translation(-v);

    return Transformation3D(tm, itm);
}

Transformation3D transformations::rotate_around_x(const Angle a)
{
	const Matrix4x4 tm = transformation_matrices::rotation_around_x(a);
	const auto itm = transformation_matrices::rotation_around_x(-a);

	return Transformation3D(tm, itm);
}

Transformation3D transformations::rotate_around_y(const Angle a)
{
	const auto tm = transformation_matrices::rotation_around_y(a);
	const auto itm = transformation_matrices::rotation_around_y(-a);

	return Transformation3D(tm, itm);
}

Transformation3D transformations::rotate_around_z(const Angle a)
{
	const auto tm = transformation_matrices::rotation_around_z(a);
	const auto itm = transformation_matrices::rotation_around_z(-a);

	return Transformation3D(tm, itm);
}

Transformation3D transformations::scale(const double sx, const double sy, const double sz)
{
	const auto tm = transformation_matrices::scaling(sx, sy, sz);
	const auto itm = transformation_matrices::scaling(-sx, -sy, -sz);

	return Transformation3D(tm, itm);
}
