#!/usr/bin/env python3

import sys

myfile = open(sys.argv[1], "r")
result = open(sys.argv[2], "w")

vertices = []
normals = []
textures = []

for line in myfile:
    l = line.split()
    if l[0] == "v":
        vertices.append("\t".join([l[1], l[2], l[3]]))

    elif l[0] == "vt":
        textures.append("\t".join([l[1], l[2]]))

    elif l[0] == "vn":
        normals.append("\t".join([l[1], l[2], l[3]]))

    elif l[0] == "f":
        for i in range(1, len(l)):
            f = l[i].split("/")
            result.write("\t".join([
                vertices[int(f[0])-1],
                normals[int(f[2])-1],
                textures[int(f[1])-1],
                "\n"
            ]))
        result.write("\n")

myfile.close()
result.close()
