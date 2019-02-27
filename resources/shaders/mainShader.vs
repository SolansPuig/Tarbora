#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 size;
uniform vec2 uv;

out vec2 TexCoord;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);

    if (aTexCoord.x == 0)
        TexCoord.x = 0 + uv.x;
    else if (aTexCoord.x == 1)
        TexCoord.x = size.z + uv.x;
    else if (aTexCoord.x == 2)
        TexCoord.x = size.z + size.x + uv.x;
    else if (aTexCoord.x == 3)
        TexCoord.x = size.z + 2*size.x + uv.x;
    else
        TexCoord.x = 2*size.z + 2*size.x + uv.x;

    if (aTexCoord.y == 0)
        TexCoord.y = 0 + uv.y;
    else if (aTexCoord.y == 1)
        TexCoord.y = size.z + uv.y;
    else
        TexCoord.y = size.z + size.y + uv.y;
}
