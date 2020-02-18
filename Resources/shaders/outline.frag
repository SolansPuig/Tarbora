#version 330 core
out vec4 FragColor;

in vec3 EdgeDistance;

void main()
{
    float nearDistance = min(min(EdgeDistance[0],EdgeDistance[1]),EdgeDistance[2]);
    float edgeIntensity = exp2(-0.1*nearDistance*nearDistance);

    FragColor = edgeIntensity * vec4(1, 1, 1, 1);
}
