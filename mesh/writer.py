class Writer:
    def __init__(self, file, reader, hierarchy):
        file = open(file, "w")
        file.write(reader.serialize())
        file.write("\nBoxes\n")
        file.write(hierarchy.serialize())
