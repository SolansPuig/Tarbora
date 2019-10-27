#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aFace;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;
uniform vec2 uv;
uniform vec3 size;
uniform sampler2D myTexture;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

void main()
{
    vec4 viewPos = view * transform * vec4(aPos, 1.0f);
    FragPos = viewPos.xyz;
    Normal = transpose(inverse(mat3(view * transform))) * aNormal;
    gl_Position = projection * viewPos;

    ivec2 texSize = textureSize(myTexture, 0);
    // float xoffset = 0.5 / texSize.x;
    // float yoffset = 0.5 / texSize.y;
    float xoffset = 0;
    float yoffset = 0;

    if (aTexCoord.x == 0)
    {
        TexCoord.x = xoffset + uv.x;
    }
    else if (aTexCoord.x == 1)
    {
        if (aFace == 1 || aFace == 3) xoffset *= -1;
        TexCoord.x = size.z + uv.x + xoffset;
    }
    else if (aTexCoord.x == 2)
    {
        if (aFace == 4 || aFace == 0) xoffset *= -1;
        TexCoord.x = size.z + size.x + uv.x + xoffset;
    }
    else if (aTexCoord.x == 3)
    {
        if (aFace == 5 || aFace == 2) xoffset *= -1;
        TexCoord.x = size.z + 2*size.x + uv.x + xoffset;
    }
    else
    {
        TexCoord.x = 2*size.z + 2*size.x + uv.x - xoffset ;
    }

    if (aTexCoord.y == 0)
    {
        TexCoord.y = uv.y + yoffset;
    }
    else if (aTexCoord.y == 1)
    {
        if (aFace == 4 || aFace == 5) yoffset *= -1;
        TexCoord.y = size.z + uv.y + yoffset;
    }
    else
    {
        TexCoord.y = size.z + size.y + uv.y - yoffset;
    }
}
