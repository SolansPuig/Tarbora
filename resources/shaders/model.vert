#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 transform;

out vec2 TexCoord;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0f);

    if (aTexCoord.x == 0)
        TexCoord.x = 0;
    else if (aTexCoord.x == 1)
        TexCoord.x = 0.25;
    else if (aTexCoord.x == 2)
        TexCoord.x = 0.5;
    else if (aTexCoord.x == 3)
        TexCoord.x = 0.75;
    else
        TexCoord.x = 1;

    if (aTexCoord.y == 0)
        TexCoord.y = 0;
    else if (aTexCoord.y == 1)
        TexCoord.y = 0.5;
    else
        TexCoord.y = 1;
}
