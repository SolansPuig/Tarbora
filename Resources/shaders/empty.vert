#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aVertex;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    float vertex = int(aVertex) % 4;
    TexCoords.x = (vertex == 0 || vertex == 3) ? 0 : 1;
    TexCoords.y = (vertex == 0 || vertex == 1) ? 0 : 1;
}
