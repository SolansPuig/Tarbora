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
uniform sampler2D myTexture;
uniform int pixelDensity;

out vec2 TexPos;
out vec2 TexSize;
out vec2 TexCoords;
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

    vec3 meshSize = aMeshSize * pixelDensity;
    vec3 texSize = aTexSize * pixelDensity;
    float face = floor(aVertex / 4);
    float vertex = int(aVertex) % 4;
    bool vertexX = (vertex == 0 || vertex == 3) ? true : false; // The vertex is on the left side?
    bool vertexY = (vertex == 0 || vertex == 1) ? true : false; // The vertex is on the up side?

    switch (int(face)) {
        case 0:
            TexPos.xy = aUv.xy + texSize.z;
            TexSize.xy = texSize.xy;
            TexCoords.x = vertexX ? 0 : meshSize.x;
            TexCoords.y = vertexY ? 0 : meshSize.y;
            break;
        case 1:
            TexPos.x = aUv.x + texSize.z + 2*texSize.x;
            TexPos.y = aUv.y + texSize.z;
            TexSize.xy = texSize.zy;
            TexCoords.x = vertexX ? 0 : meshSize.z;
            TexCoords.y = vertexY ? 0 : meshSize.y;
            break;
        case 2:
            TexPos.x = aUv.x + texSize.z + texSize.x;
            TexPos.y = aUv.y + texSize.z;
            TexSize.xy = texSize.xy;
            TexCoords.x = vertexX ? meshSize.x : 0;
            TexCoords.y = vertexY ? 0 : meshSize.y;
            break;
        case 3:
            TexPos.x = aUv.x;
            TexPos.y = aUv.y + texSize.z;
            TexSize.xy = texSize.zy;
            TexCoords.x = vertexX ? 0 : meshSize.z;
            TexCoords.y = vertexY ? 0 : meshSize.y;
            break;
        case 4:
            TexPos.x = aUv.x + texSize.z;
            TexPos.y = aUv.y;
            TexSize.xy = texSize.xz;
            TexCoords.x = vertexX ? 0 : meshSize.x;
            TexCoords.y = vertexY ? 0 : meshSize.z;
            break;
        case 5:
            TexPos.x = aUv.x + texSize.z + texSize.x;
            TexPos.y = aUv.y;
            TexSize.xy = texSize.xz;
            TexCoords.x = vertexX ? 0 : meshSize.x;
            TexCoords.y = vertexY ? 0 : meshSize.z;
            break;
    }

    TexPos /= textureSize(myTexture, 0);
    TexSize /= textureSize(myTexture, 0);
    TexCoords /= textureSize(myTexture, 0);
}
