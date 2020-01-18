#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aFace;
layout (location = 4) in mat4 aTransform;
layout (location = 8) in vec2 aUv;
layout (location = 9) in vec3 aMeshSize;
layout (location = 10) in vec3 aTexSize;
layout (location = 11) in vec3 aPrimary;
layout (location = 12) in vec3 aSecondary;
layout (location = 13) in vec3 aDetail;
layout (location = 14) in vec3 aDetail2;

uniform mat4 projection;
uniform mat4 view;
uniform sampler2D myTexture;
uniform int pixelDensity;

out vec2 TexPos;
out vec2 TexSize;
out vec2 TexCoordXY;
out vec2 TexCoordXZ;
out vec2 TexCoordYZ;
out vec3 TexNormal;
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

    vec3 texSize = aTexSize * pixelDensity;
    vec2 uv = aUv;

    vec3 TexCoords = vec3(aTransform * vec4(aPos, 1.0f)).xyz * pixelDensity;
    TexNormal =  vec3(transpose(inverse(aTransform)) * vec4(aNormal, 0.0));
    TexCoordXY = TexCoords.xy / textureSize(myTexture, 0);
    TexCoordXZ = TexCoords.xz / textureSize(myTexture, 0);
    TexCoordYZ = TexCoords.zy / textureSize(myTexture, 0);
    TexCoordXY.x *= sign(TexNormal.z);
    TexCoordXY.y *= -1;
    TexCoordYZ.x *= -sign(TexNormal.x);
    TexCoordYZ.y *= -1;
    TexCoordXZ.x *= sign(TexNormal.y);

    switch (int(aFace)) {
        case 0:
            TexPos.x = uv.x + texSize.z;
            TexPos.y = uv.y + texSize.z;
            TexSize.x = texSize.x;
            TexSize.y = texSize.y;
            break;
        case 1:
            TexPos.x = uv.x + texSize.z + 2*texSize.x;
            TexPos.y = uv.y + texSize.z;
            TexSize.x = texSize.z;
            TexSize.y = texSize.y;
            break;
        case 2:
            TexPos.x = uv.x + texSize.z + texSize.x;
            TexPos.y = uv.y + texSize.z;
            TexSize.x = texSize.x;
            TexSize.y = texSize.y;
            break;
        case 3:
            TexPos.x = uv.x;
            TexPos.y = uv.y + texSize.z;
            TexSize.x = texSize.z;
            TexSize.y = texSize.y;
            break;
        case 4:
            TexPos.x = uv.x + texSize.z;
            TexPos.y = uv.y;
            TexSize.x = texSize.x;
            TexSize.y = texSize.z;
            break;
        case 5:
            TexPos.x = uv.x + texSize.z + texSize.x;
            TexPos.y = uv.y;
            TexSize.x = texSize.x;
            TexSize.y = texSize.z;
            break;
    }

    TexPos /= textureSize(myTexture, 0);
    TexSize /= textureSize(myTexture, 0);
}
