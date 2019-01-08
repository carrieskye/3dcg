from reader import *
from writer import *
from boundingbox import *


def convert(file_name, max_depth, min_triangles):
    reader = Reader("../samples/" + file_name + ".mesh")

    BoundingBox.index = len(reader.vertices) + len(reader.triangles) + 5
    BoundingBox.max_depth = max_depth
    BoundingBox.min_triangles = min_triangles

    bounding_box_hierarchy = BoundingBox.get_bb_of_triangles(reader.triangles, 1)
    Writer("../results/" + file_name + "-" + str(max_depth) + ".mesh", reader, bounding_box_hierarchy)


# USAGE
# convert(<name_of_the_file>, <how_many_levels_you_want_in_the_tree>, <min_number_of_triangles_in_bottom_boxes>)
# convert("armadillo", 20, 3)
