#version 330 core
out vec4 FragColor;

in vec3 pos;
in vec2 TexCoord;

// vec3 skyColor = vec3(150, 222, 255)/255;
// vec3 horizonColor = vec3(204, 204, 204)/255;
// float horizonHeight = 1.0;
// float exponent = 15;

vec3 skyColor = vec3(158, 215, 255)/255;
vec3 horizonColor = vec3(255, 131, 112)/255;
float horizonHeight = 1.0;
float exponent = 3;

struct Sun {
    vec3 color;
    vec3 pos;
    float size;
    float sharpness;
};

void main()
{
    int sunNumber = 2;
    Sun sun[5];
    sun[0].color = vec3(255, 247, 232)/255;
    sun[0].pos = normalize(vec3(0.13, 0.21, -1.0));
    sun[0].size = 7;
    sun[0].sharpness = 14;

    sun[1].color = vec3(255, 212, 125)/255;
    sun[1].pos = normalize(vec3(0.3, 0.04, -1.0));
    sun[1].size = 8;
    sun[1].sharpness = 10;

    vec3 p = normalize(pos);
    float height = pow(min(1.0, horizonHeight - p.y), exponent);
    vec3 color = mix(skyColor, horizonColor, height);

    for (int i = 0; i < sunNumber; ++i)
    {
        float d = clamp(pow(abs(distance(p, sun[i].pos))*128/sun[i].size, sun[i].sharpness), 0.0, 1.0);
        color = mix(sun[i].color, color, d);
    }
    FragColor = vec4(color, 1.0);
}
