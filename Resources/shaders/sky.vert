#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in float aVertex;
layout (location = 3) in mat4 aTransform;

uniform mat4 projection;
uniform mat4 view;
uniform sampler2D myTexture;

out vec3 pos;

void main()
{
    mat4 skyView = view;
    skyView[3] = vec4(0.f);
    gl_Position = (projection * skyView * aTransform * vec4(aPos, 1.0f)).xyww;
    pos = aPos;
}
