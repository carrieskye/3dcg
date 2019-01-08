#include "bounding-box-hierarchy.h"
#include <fstream>
#include <string>
#include "easylogging++.h"
#include "triangle-primitive.h"
#include "bounding-box-accelerator.h"

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
Point3D parseOwnPoint3D(const string point)
{
	auto coords = split(point, ';');
	return Point3D(
		stod(coords.at(0)),
		stod(coords.at(1)),
		stod(coords.at(2))
	);
}


//int got_in_all_hits = 0;
namespace
{

	class BoundingBoxHierarchy : public primitives::_private_::PrimitiveImplementation
	{
	public:
		bool is_root_box = false;
		Box box;
		string box_id;
		vector<Primitive> primitives;
		vector<BoundingBoxHierarchy> sub_boxes;
		BoundingBoxHierarchy(const Box box, vector<Primitive> primitives, vector<BoundingBoxHierarchy> sub_boxes, string box_id) :
			box(box), primitives(primitives), sub_boxes(sub_boxes), box_id(box_id) {}

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
					read_vertices[line_split[0]] = parseOwnPoint3D(line_split[1]);
					break;
				case ReadingTriangles:
					line_split = split(line, ' ');
					line_split2 = split(line_split[1], ';');
					read_triangles[line_split[0]] = primitives::triangle(
						read_vertices[line_split2[0]],
						read_vertices[line_split2[1]],
						read_vertices[line_split2[2]]
					);
					//LOG(INFO) << "T " << read_vertices[line_split2[0]] << " " << read_vertices[line_split2[1]] << " " << read_vertices[line_split2[2]];
					break;
				case ReadingBoxes:
					line_split = split(line, ' ');
					read_boxes[line_split[0]] = ReadingBox(
						Box::from_corners(
							parseOwnPoint3D(line_split[1]),
							parseOwnPoint3D(line_split[2])
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

			function<BoundingBoxHierarchy(string)> f;
			f = [&f, read_boxes, read_triangles, root_box_id](string ref) -> BoundingBoxHierarchy
			{
				auto a = read_boxes.find(ref)->second;
				Box b = a.box;
				vector<Primitive> p;
				vector<BoundingBoxHierarchy> sb;
				for (auto r : a.refs)
				{
					if (stoi(r) > stoi(root_box_id)) sb.push_back(f(r));
					else p.push_back(
						read_triangles.find(r)->second
					);
				}
				return BoundingBoxHierarchy(b, p, sb, ref);
			};
			auto bb = f(root_box_id);
			this->box = bb.box;
			this->primitives = bb.primitives;
			this->sub_boxes = bb.sub_boxes;
			this->is_root_box = true;
			this->box_id = bb.box_id;

			/*
			std::vector<int> primitives_in_box = vector<int>();
			function<void(BoundingBoxHierarchy, int)> print_tree;
			print_tree = [&print_tree, primitives_in_box](BoundingBoxHierarchy bbh, int level) -> void
			{
				LOG(INFO) << std::string(level, ' ') << "b";
				for (auto bbh_b : bbh.sub_boxes)
				{
					print_tree(bbh_b, level+1);
				}
				for (auto bbh_p : bbh.primitives)
				{
					//LOG(INFO) << std::string(level, ' ') << "p";
				}
				if (bbh.primitives.size() > 0)
					primitives_in_box.push
			};
			print_tree(bb, 0);
			*/
		}

		bool find_first_positive_hit(const Ray& ray, Hit* output_hit) const override
		{
			if (!box.is_hit_positively_by(ray)) return false;
			bool o = false;
			if (!sub_boxes.empty())
			{
				for (auto sub_box : sub_boxes)
					if (sub_box.find_first_positive_hit(ray, output_hit))
						o = true;
			}
			else if (!primitives.empty())
			{
				bool o = false;
				for (auto primitive : primitives)
				{
					if (primitive->bounding_box().is_hit_positively_by(ray))
					{
						if (primitive->find_first_positive_hit(ray, output_hit))
							o = true;
					}
				}
				return o;
			}
			else LOG(INFO) << "SHOULD NOT HAPPEN";
			return o;
		}


		vector<shared_ptr<Hit>> find_all_hits(const Ray& ray) const override
		{
			//LOG(INFO) << "box " << this->box_id;
			vector<shared_ptr<Hit>> hits = vector<shared_ptr<Hit>>();
			// If this box is not hit, we don't need to look futher
			if (!box.is_hit_by(ray))
			{
				//LOG(INFO) << "  No hit!";
				// We return if there's no intersection.
				return hits;
			}
			// check if there are subboxes
			if (!sub_boxes.empty())
			{
				//LOG(INFO) << "  checking boxes";
				for (auto b : sub_boxes)
				{
					auto subbox_hits = b.find_all_hits(ray);
					if (subbox_hits.size() > 0)
						hits.insert(hits.end(), subbox_hits.begin(), subbox_hits.end());
				}
				if (this->is_root_box && hits.size() > 0) {
					//LOG(INFO) << " got_in_all_hits = " << got_in_all_hits;
					//LOG(INFO) << "  Sorting " << hits.size() << " hits";
					std::sort(hits.begin(), hits.end(), [](const std::shared_ptr<Hit>& a, const std::shared_ptr<Hit>& b) { return a->t < b->t; });
				}
				return hits;
			}
			// if there are no subboxes we need to check the primitives
			if (!primitives.empty())
			{
				//LOG(INFO) << "  checking primitives";
				Hit hit;
				for (auto p : primitives)
				{
					//got_in_all_hits++;
					if (p->find_first_positive_hit(ray, &hit))
						hits.push_back(make_shared<Hit>(hit));
					//LOG(INFO) << "got hits " << primitive_hits.size();
					//for (auto h : primitive_hits) hits.push_back(h);
					//if (primitive_hits.size() > 0)
						//hits.insert(hits.end(), primitive_hits.begin(), primitive_hits.end());
				}
			}
			else
				LOG(INFO) << "THIS SHOULD NOT HAPPEN";
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
