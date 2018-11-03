#include "math/transformation3d.h"
#include "math/transformation-matrices.h"

using namespace math;


Transformation3D transformations::translation(const Vector3D& v)
{
    Matrix4x4 tm = transformation_matrices::translation(v);
    Matrix4x4 itm = transformation_matrices::translation(-v);

    return Transformation3D(tm, itm);
}
