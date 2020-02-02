#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in float aVertex;
layout (location = 3) in mat4 aTransform;
layout (location = 7) in vec2 aUv;
layout (location = 8) in vec3 aMeshSize;
layout (location = 9) in vec3 aTexSize;
layout (location = 10) in vec3 aPrimary;
layout (location = 11) in vec3 aSecondary;
layout (location = 12) in vec3 aDetail;
layout (location = 13) in vec3 aDetail2;

uniform mat4 projection;
uniform mat4 view;
uniform sampler2D albedo;

out vec2 TexCoord;

void main()
{
    gl_Position = projection * view * aTransform * vec4(aPos, 1.0f);

    ivec2 texSize = textureSize(albedo, 0);
    float xoffset = 0.5 / texSize.x;
    float yoffset = 0.5 / texSize.y;

    if (aTexCoord.x == 0)
    {
        TexCoord.x = xoffset + aUv.x;
    }
    else if (aTexCoord.x == 1)
    {
        if (aFace == 1 || aFace == 3) xoffset *= -1;
        TexCoord.x = aTextureSize.z + aUv.x + xoffset;
    }
    else if (aTexCoord.x == 2)
    {
        if (aFace == 4 || aFace == 0) xoffset *= -1;
        TexCoord.x = aTextureSize.z + aTextureSize.x + aUv.x + xoffset;
    }
    else if (aTexCoord.x == 3)
    {
        if (aFace == 5 || aFace == 2) xoffset *= -1;
        TexCoord.x = aTextureSize.z + 2*aTextureSize.x + aUv.x + xoffset;
    }
    else
    {
        TexCoord.x = 2*aTextureSize.z + 2*aTextureSize.x + aUv.x - xoffset ;
    }

    if (aTexCoord.y == 0)
    {
        TexCoord.y = aUv.y + yoffset;
    }
    else if (aTexCoord.y == 1)
    {
        if (aFace == 4 || aFace == 5) yoffset *= -1;
        TexCoord.y = aTextureSize.z + aUv.y + yoffset;
    }
    else
    {
        TexCoord.y = aTextureSize.z + aTextureSize.y + aUv.y - yoffset;
    }
}
