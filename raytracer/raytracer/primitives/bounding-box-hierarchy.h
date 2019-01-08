#pragma once

#include "primitives/primitive.h"
#include <string>
#include <sstream>
using namespace std;
using namespace math;
namespace raytracer
{
	namespace primitives
	{
		Primitive load_mesh(std::string path);
	}
}
vector<string> split(const string& s, char delimiter);
Point3D parseOwnPoint3D(const string point);
class ReadingBox
{
public:
	Box box;
	vector<string> refs;
	ReadingBox(Box box, vector<string> refs) : box(box), refs(refs) {}
	ReadingBox() : box(math::Box::empty()) {}
};
enum ReadingMeshState { ReadingVertices, ReadingTriangles, ReadingBoxes };

