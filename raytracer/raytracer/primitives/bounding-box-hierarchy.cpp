#include "bounding-box-hierarchy.h"
#include <fstream>
#include <string>
#include "easylogging++.h"
#include "triangle-primitive.h"

using namespace raytracer;
using namespace math;
using namespace std;


// https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
vector<string> split(const string& s, char delimiter)
{
	vector<string> tokens;
	string token;
	istringstream tokenStream(s);
	while (getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

// 1.0;1.0;0.0 -> Point3D(1.0, 1.0, 0.0)
Point3D parsePoint3D(const string point)
{
	auto coords = split(point, ';');
	return Point3D(
		stod(coords.at(0)),
		stod(coords.at(1)),
		stod(coords.at(2))
	);
}

class ReadingBox
{
public:
	Box box;
	vector<string> refs;
	ReadingBox(Box box, vector<string> refs) : box(box), refs(refs) {}
	ReadingBox() : box(Box::empty()) {}
};

namespace
{
	enum ReadingMeshState { ReadingVertices, ReadingTriangles, ReadingBoxes };

	class BoundingBoxHierarchy : public primitives::_private_::PrimitiveImplementation
	{
	public:
		Box box;
		vector<Primitive> primitives;
		vector<BoundingBoxHierarchy> sub_boxes;
		BoundingBoxHierarchy(const Box box, vector<Primitive> primitives, vector<BoundingBoxHierarchy> sub_boxes) :
			box(box), primitives(primitives), sub_boxes(sub_boxes) {}

		BoundingBoxHierarchy(const string path) : box(Box::empty())
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
				return;
			}
			while (getline(myfile, line))
			{
				//LOG(INFO) << line;
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
					read_vertices[line_split[0]] = parsePoint3D(line_split[1]);
					break;
				case ReadingTriangles:
					line_split = split(line, ' ');
					line_split2 = split(line_split[1], ';');
					read_triangles[line_split[0]] = primitives::triangle(
						read_vertices[line_split2[0]],
						read_vertices[line_split2[1]],
						read_vertices[line_split2[2]]
					);
					break;
				case ReadingBoxes:
					line_split = split(line, ' ');
					read_boxes[line_split[0]] = ReadingBox(
						Box::from_corners(
							parsePoint3D(line_split[1]),
							parsePoint3D(line_split[2])
						),
						split(line_split[3], ';')
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
			box = read_boxes.find(root_box_id)->second.box;
			for (auto rt : read_triangles)
			{
				primitives.push_back(rt.second);
			}
		}

		vector<shared_ptr<Hit>> find_all_hits(const Ray& ray) const override
		{
			vector<shared_ptr<Hit>> hits = vector<shared_ptr<Hit>>();
			// If this box is not hit, we don't need to look futher
			if (!box.is_hit_positively_by(ray))
			{
				// We return if there's no intersection.
				return hits;
			}
			// check if there are subboxes
			if (!sub_boxes.empty())
			{
				for (auto b : sub_boxes)
				{
					auto subbox_hits = b.find_all_hits(ray);
					hits.insert(hits.end(), subbox_hits.begin(), subbox_hits.end());
				}
				return hits;
			}
			// if there are no subboxes we need to check the primitives
			if (!primitives.empty())
			{
				for (auto p : primitives)
				{
					auto primitive_hits = p->find_all_hits(ray);
					hits.insert(hits.end(), primitive_hits.begin(), primitive_hits.end());
				}
			}
			return hits;
		}
		Box bounding_box() const override
		{
			return box;
		}
	};
}

Primitive primitives::load_mesh(const string path)
{
	return Primitive(make_shared<BoundingBoxHierarchy>(path));
}
