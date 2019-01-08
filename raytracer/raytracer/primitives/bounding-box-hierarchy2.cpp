#include "bounding-box-hierarchy2.h"
#include <fstream>
#include <string>
#include "easylogging++.h"
#include "triangle-primitive.h"
#include "bounding-box-accelerator.h"
#include <numeric>
#include "union-primitive.h"
#include "bounding-box-hierarchy.h"

using namespace raytracer;
using namespace math;
using namespace std;
using namespace raytracer;
using namespace std;



Primitive primitives::bounding_box_hierarchy2(const string path)
{
	LOG(INFO) << "Loading mesh from: " << path;
	ReadingMeshState state = ReadingVertices;
	string line;

	map<string, Point3D> read_vertices;
	map<string, Primitive> read_triangles;
	map<string, ReadingBox> read_boxes;

	vector<string> line_split;
	vector<string> line_split2;

	string root_box_id;

	ifstream myfile(path);
	if (!myfile.is_open()) {
		LOG(INFO) << "Unable to open file";
	}
	while (getline(myfile, line))
	{
		if (line == "Vertices")
		{
			state = ReadingVertices;
			LOG(INFO) << "Reading vertices";
		}
		else if (line == "Triangles")
		{
			state = ReadingTriangles;
			LOG(INFO) << "Reading triangles";
		}
		else if (line == "Boxes")
		{
			state = ReadingBoxes;
			LOG(INFO) << "Reading boxes";
		}
		else if (line.length() == 0) { /* ignore */ }
		else switch (state)
		{
		case ReadingVertices:
			line_split = split(line, ' ');
			read_vertices[line_split[0]] = parseOwnPoint3D(line_split[1]);
			break;
		case ReadingTriangles:
			line_split = split(line, ' ');
			line_split2 = split(line_split[1], ';');
			read_triangles[line_split[0]] = triangle(
				read_vertices[line_split2[0]],
				read_vertices[line_split2[1]],
				read_vertices[line_split2[2]]
			);
			break;
		case ReadingBoxes:
			line_split = split(line, ' ');
			read_boxes[line_split[0]] = ReadingBox(
				Box::empty(),
				split(line_split[1], ';')
			);
			if (root_box_id.size() == 0)
			{
				root_box_id = line_split[0];
			}
			break;
		}
	}
	myfile.close();
	LOG(INFO) << "We loaded " << read_vertices.size() << " vertices" <<
		" and " << read_triangles.size() << " triangles" <<
		" and " << read_boxes.size() << " boxes.";
	LOG(INFO) << "ROOT BOX " << root_box_id;
	function<Primitive(string)> f;
	f = [&f, read_boxes, read_triangles, root_box_id](const string ref) -> Primitive
	{
		auto a = read_boxes.find(ref)->second;
		vector<Primitive> p;

		for(auto r : a.refs)
		{
			if (read_triangles.find(r) == read_triangles.end()) p.push_back(f(r));
			else p.push_back(read_triangles.find(r)->second);
		}
		if (p.size() == 1) return p.at(0);
		return bounding_box_accelerator(make_union(p));
	};
	LOG(INFO) << "BUILD TREE";
	return f(root_box_id);
}