class Vertex:
    index = 1

    def __init__(self, points):
        Vertex.index += 1
        self.x = points[0]
        self.y = points[1]
        self.z = points[2]
        self.index = Vertex.index

    def __str__(self):
        return str("(" + str(self.x) + ", " + str(self.y) + ", " + str(self.z) + ")")

    def __repr__(self):
        return self.__str__()

    def serialize_w_index(self, separator=";"):
        return str(self.index) + " " + str(self.x) + separator + str(self.y) + separator + str(self.z)

    def serialize(self, separator=";"):
        return str(self.x) + separator + str(self.y) + separator + str(self.z)
