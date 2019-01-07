from vertex import *


class Triangle:
    index = 0

    def __init__(self, p1, p2, p3):
        Triangle.index += 1
        self.p1 = p1
        self.p2 = p2
        self.p3 = p3
        self.index = Triangle.index

    def get_min(self):
        return Vertex([min(self.p1.x, self.p2.x, self.p3.x),
                       min(self.p1.y, self.p2.y, self.p3.y),
                       min(self.p1.z, self.p2.z, self.p3.z)])

    def get_max(self):
        return Vertex([max(self.p1.x, self.p2.x, self.p3.x),
                       max(self.p1.y, self.p2.y, self.p3.y),
                       max(self.p1.z, self.p2.z, self.p3.z)])

    def __str__(self):
        return str(self.p1) + ", " + str(self.p2) + ", " + str(self.p3)

    def __repr__(self):
        return self.__str__()

    def serialize(self, separator=";"):
        return str(self.index) + " " + str(self.p1.index) + separator + str(self.p2.index) + separator \
               + str(self.p3.index)
