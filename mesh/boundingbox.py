from triangle import *


class BoundingBox:
    index = None
    max_depth = None
    min_triangles = None

    def __init__(self, bb_min, bb_max, triangles, depth):
        BoundingBox.index += 1
        self.min = bb_min
        self.max = bb_max
        self.triangles = triangles
        self.depth = depth
        self.index = BoundingBox.index
        self.sub_boxes = []
        if len(triangles) >= BoundingBox.min_triangles and depth < BoundingBox.max_depth:
            self.sub_boxes = BoundingBox.get_sub_boxes(self)

    def __str__(self):
        string = "BB " + str(self.index)
        string += ", min = " + str(self.min)
        string += ", max = " + str(self.max)
        string += ", depth = " + str(self.depth)
        if len(self.sub_boxes) > 0:
            string += ", " + str(len(self.sub_boxes)) + " sub boxes:"
            for bb in self.sub_boxes:
                string += " " + str(bb.index)
            for bb in self.sub_boxes:
                string += "\n" + str(bb)
        if len(self.sub_boxes) == 0:
            string += ", " + str(len(self.triangles)) + " triangles"
            for triangle in self.triangles:
                string += "\n" + "   T: " + str(triangle)
        return string

    def serialize(self, separator=" "):
        string = str(self.index) + separator + self.min.serialize() + separator + self.max.serialize() + separator
        if len(self.sub_boxes) > 0:
            for bb in self.sub_boxes:
                string += str(bb.index) + ";"
            string = string[:-1]
            for bb in self.sub_boxes:
                string += "\n" + bb.serialize()
        else:
            for triangle in self.triangles:
                string += str(triangle.index) + ";"
            string = string[:-1]
        return string

    @staticmethod
    def get_bb_of_triangles(triangles, depth):
        bb_min = Vertex([triangles[0].p1.x, triangles[0].p1.y, triangles[0].p1.z])
        bb_max = Vertex([triangles[0].p1.x, triangles[0].p1.y, triangles[0].p1.z])

        for triangle in triangles:
            triangle_min = triangle.get_min()
            triangle_max = triangle.get_max()

            if triangle_min.x < bb_min.x:
                bb_min.x = triangle_min.x
            if triangle_min.y < bb_min.y:
                bb_min.y = triangle_min.y
            if triangle_min.z < bb_min.z:
                bb_min.z = triangle_min.z

            if triangle_max.x > bb_max.x:
                bb_max.x = triangle_max.x
            if triangle_max.y > bb_max.y:
                bb_max.y = triangle_max.y
            if triangle_max.z > bb_max.z:
                bb_max.z = triangle_max.z

        return BoundingBox(bb_min, bb_max, triangles, depth)

    @staticmethod
    def get_sub_boxes(box):
        len_x = box.max.x - box.min.x
        len_y = box.max.y - box.min.y
        len_z = box.max.z - box.min.z

        if len_x >= len_y and len_x >= len_z:
            left_max = Vertex([box.max.x - len_x / 2, box.max.y, box.max.z])
            right_min = Vertex([box.min.x + len_x / 2, box.min.y, box.min.z])
        elif len_y >= len_z:
            left_max = Vertex([box.max.x, box.max.y - len_y / 2, box.max.z])
            right_min = Vertex([box.min.x, box.min.y + len_y / 2, box.min.z])
        else:
            left_max = Vertex([box.max.x, box.max.y, box.max.z - len_z / 2])
            right_min = Vertex([box.min.x, box.min.y, box.min.z + len_z / 2])

        left = []
        right = []
        leftovers = []

        for triangle in box.triangles:
            if BoundingBox.triangle_fits_min_max(box.min, left_max, triangle):
                left.append(triangle)
            elif BoundingBox.triangle_fits_min_max(right_min, box.max, triangle):
                right.append(triangle)
            else:
                leftovers.append(triangle)

        sub_boxes = []

        if len(left) > 0:
            sub_boxes.append(BoundingBox(box.min, left_max, left, box.depth + 1))
        if len(right) > 0:
            sub_boxes.append(BoundingBox(right_min, box.max, right, box.depth + 1))
        if len(leftovers) > 0:
            sub_boxes.append(BoundingBox.get_bb_of_triangles(leftovers, box.depth + 1))

        return sub_boxes

    @staticmethod
    def triangle_fits_min_max(bb_min, bb_max, triangle):
        triangle_min = triangle.get_min()
        triangle_max = triangle.get_max()

        min_fits = triangle_min.x >= bb_min.x and triangle_min.y >= bb_min.y and triangle_min.z >= bb_min.z
        max_fits = triangle_max.x <= bb_max.x and triangle_max.y <= bb_max.y and triangle_max.z <= bb_max.z

        return min_fits and max_fits
