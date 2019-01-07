from triangle import *
from vertex import *


class Reader:
    def __init__(self, file):
        file = open(file, "r")

        nb_of_vertices = int(file.readline())
        self.vertices = []

        for i in range(0, nb_of_vertices):
            vertex = Vertex(list(map(float, file.readline().split())))
            self.vertices.append(vertex)

        Triangle.index = len(self.vertices) + 3

        nb_of_triangles = int(file.readline())
        self.triangles = []

        for j in range(0, nb_of_triangles):
            indices = list(map(int, file.readline().split()))
            self.triangles.append(
                Triangle(self.vertices[indices[0]], self.vertices[indices[1]], self.vertices[indices[2]]))

    def __str__(self):
        string = "Vertices (" + str(len(self.vertices)) + ")\n"
        for vertex in self.vertices:
            string += str(vertex) + "\n"

        string += "\nTriangles (" + str(len(self.triangles)) + ")\n"
        for triangle in self.triangles:
            string += str(triangle) + "\n"
        return string

    def serialize(self):
        string = "Vertices\n"
        for vertex in self.vertices:
            string += vertex.serialize_w_index() + "\n"
        string += "\nTriangles\n"
        for triangle in self.triangles:
            string += triangle.serialize() + "\n"
        return string
