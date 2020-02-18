#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 3) in mat4 aTransform;

uniform mat4 projection;
uniform mat4 view;
uniform sampler2D myTexture;
uniform int pixelDensity;

out vec3 FragPos;
out vec3 Normal;

void main()
{
    vec4 pos = view * aTransform * vec4(aPos, 1.0f);
    FragPos = pos.xyz;
    Normal = vec3(transpose(inverse(view * aTransform)) * vec4(aNormal, 0.0));
    gl_Position = projection * pos;
}
