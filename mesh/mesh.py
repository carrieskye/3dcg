from reader import *
from writer import *
from boundingbox import *
import sys
import threading

sys.setrecursionlimit(2 ** 20)

threading.stack_size(67108864)  # 64MB stack
sys.setrecursionlimit(2 ** 20)  # something real big


# you actually hit the 64MB limit first
# going by other answers, could just use 2**32-1

def convert(file_name, max_depth, min_triangles):
    reader = Reader("../samples/" + file_name + ".mesh")

    BoundingBox.index = len(reader.vertices) + len(reader.triangles) + 5
    BoundingBox.max_depth = max_depth
    BoundingBox.min_triangles = min_triangles

    bounding_box_hierarchy = BoundingBox.get_bb_of_triangles(reader.triangles, 1)
    Writer("../results/" + file_name + "-" + str(max_depth) + ".mesh", reader, bounding_box_hierarchy)


# prevent RecursionError: maximum recursion depth exceeded
# https://stackoverflow.com/a/2918118

# USAGE
# convert(<name_of_the_mesh>, <how_many_levels_you_want_in_the_tree>, <min_number_of_triangles_in_bottom_boxes>)
def main():
    convert("armadillo", 10, 20)


thread = threading.Thread(target=main)
thread.start()
