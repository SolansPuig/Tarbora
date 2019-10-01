#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aFace;

uniform mat4 projection;
uniform mat4 transform;
uniform sampler2D myTexture;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

void main()
{
    gl_Position = (projection * transform * vec4(aPos, 1.0f)).xyww;
    FragPos = vec3(0.0f, 0.0f, 0.f);
    Normal = glm::vec3(1.0f, 1.0f, 1.f);

    ivec2 texSize = textureSize(myTexture, 0);
    float xoffset = 0.5 / texSize.x;
    float yoffset = 0.5 / texSize.y;

    if (aTexCoord.x == 0)
    {
        TexCoord.x = xoffset;
    }
    else if (aTexCoord.x == 1)
    {
        if (aFace == 1 || aFace == 3) xoffset *= -1;
        TexCoord.x = 0.25 + xoffset;
    }
    else if (aTexCoord.x == 2)
    {
        if (aFace == 4 || aFace == 0) xoffset *= -1;
        TexCoord.x = 0.5 + xoffset;
    }
    else if (aTexCoord.x == 3)
    {
        if (aFace == 5 || aFace == 2) xoffset *= -1;
        TexCoord.x = 0.75 + xoffset;
    }
    else
    {
        TexCoord.x = 1 - xoffset;
    }

    if (aTexCoord.y == 0)
    {
        TexCoord.y = yoffset;
    }
    else if (aTexCoord.y == 1)
    {
        if (aFace == 4 || aFace == 5) yoffset *= -1;
        TexCoord.y = 0.5 + yoffset;
    }
    else
    {
        TexCoord.y = 1 - yoffset;
    }
}
