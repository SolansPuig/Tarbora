#version 330 core
out vec4 FragColor;

in vec3 pos;
in vec2 TexCoord;

vec3 skyColor = vec3(150, 222, 255)/255;
vec3 horizonColor = vec3(204, 204, 204)/255;

float exponent = 15;

void main()
{
    vec3 p = normalize(pos);
    float height = pow(min(1.0, 1.0 - p.y), exponent);
    vec3 color = mix(skyColor, horizonColor, height);
    FragColor = vec4(color, 1.0);
}
