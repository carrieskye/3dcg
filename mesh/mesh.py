from reader import *
from writer import *
from boundingbox import *
import sys
import threading


sys.setrecursionlimit(2 ** 20)

threading.stack_size(67108864) # 64MB stack
sys.setrecursionlimit(2 ** 20) # something real big
                               # you actually hit the 64MB limit first
                               # going by other answers, could just use 2**32-1

def convert(file_name, max_depth, min_triangles):
    reader = Reader("./samples/" + file_name)

    BoundingBox.index = len(reader.vertices) + len(reader.triangles) + 5
    BoundingBox.max_depth = max_depth
    BoundingBox.min_triangles = min_triangles

    bounding_box_hierarchy = BoundingBox.get_bb_of_triangles(reader.triangles, 1)
    Writer("./results/" + file_name, reader, bounding_box_hierarchy)

# prevent RecursionError: maximum recursion depth exceeded
# https://stackoverflow.com/a/2918118
def main():
    convert("dragon.mesh", 500, 20)

# only new threads get the redefined stack size
thread = threading.Thread(target=main)
thread.start()