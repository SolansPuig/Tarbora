#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aFace;
layout (location = 4) in mat4 aTransform;
layout (location = 8) in vec2 aUv;
layout (location = 9) in vec3 aSize;
layout (location = 10) in vec3 aPrimary;
layout (location = 11) in vec3 aSecondary;
layout (location = 12) in vec3 aDetail;
layout (location = 13) in vec3 aDetail2;

uniform mat4 projection;
uniform mat4 view;
uniform sampler2D myTexture;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
out vec3 colorPrimary;
out vec3 colorSecondary;
out vec3 colorDetail;
out vec3 colorDetail2;

void main()
{
    colorPrimary = aPrimary;
    colorSecondary = aSecondary;
    colorDetail = aDetail;
    colorDetail2 = aDetail2;

    vec4 pos = view * aTransform * vec4(aPos, 1.0f);
    FragPos = pos.xyz;
    Normal = vec3(transpose(inverse(view * aTransform)) * vec4(aNormal, 0.0));
    gl_Position = projection * pos;

    ivec2 texSize = textureSize(myTexture, 0);

    if (aTexCoord.x == 0)
    {
        TexCoord.x = aUv.x;
    }
    else if (aTexCoord.x == 1)
    {
        TexCoord.x = aSize.z + aUv.x;
    }
    else if (aTexCoord.x == 2)
    {
        TexCoord.x = aSize.z + aSize.x + aUv.x;
    }
    else if (aTexCoord.x == 3)
    {
        TexCoord.x = aSize.z + 2*aSize.x + aUv.x;
    }
    else
    {
        TexCoord.x = 2*aSize.z + 2*aSize.x + aUv.x ;
    }

    if (aTexCoord.y == 0)
    {
        TexCoord.y = aUv.y;
    }
    else if (aTexCoord.y == 1)
    {
        TexCoord.y = aSize.z + aUv.y;
    }
    else
    {
        TexCoord.y = aSize.z + aSize.y + aUv.y;
    }
}
