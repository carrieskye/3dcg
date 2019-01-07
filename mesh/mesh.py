from reader import *
from writer import *
from boundingbox import *


def convert(file_name, max_depth, min_triangles):
    reader = Reader("../samples/" + file_name)

    BoundingBox.index = len(reader.vertices) + len(reader.triangles) + 5
    BoundingBox.max_depth = max_depth
    BoundingBox.min_triangles = min_triangles

    bounding_box_hierarchy = BoundingBox.get_bb_of_triangles(reader.triangles, 1)
    Writer("../results/" + file_name, reader, bounding_box_hierarchy)


# USAGE
# convert("armadillo.mesh", 2, 3)
