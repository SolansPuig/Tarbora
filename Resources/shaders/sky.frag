#version 330 core
out vec4 FragColor;

in vec3 pos;
in vec2 TexCoord;

uniform vec3 horizonColor;
uniform vec3 skyColor;
uniform float horizonHeight;
uniform float exponent;

// vec3 skyColor = vec3(158, 215, 255)/255;
// vec3 horizonColor = vec3(255, 131, 112)/255;
// float horizonHeight = 1.0;
// float exponent = 3;

struct Sun {
    vec3 color;
    vec3 pos;
    float size;
    float sharpness;
};

uniform Sun sun[5];

void main()
{
    vec3 p = normalize(pos);
    float height = pow(min(1.0, horizonHeight - p.y), exponent);
    vec3 color = mix(skyColor/255.0, horizonColor/255.0, height);

    for (int i = 0; i < 5; ++i)
    {
        float d = clamp(
          pow(abs(distance(p, normalize(sun[i].pos)))*128/sun[i].size, sun[i].sharpness),
          0.0, 1.0
        );
        color = mix(sun[i].color/255.0, color, d);
    }
    FragColor = vec4(color, 1.0);
}
