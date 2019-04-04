#version 330 core

in vec3 vPosition;

float random (in vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f*f*(3.0-2.0*f);

    return mix(a, b, u.x) + (c - a)* u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

void main()
{
    vec2 st = vec2(vPosition.x * vPosition.z, vPosition.y * vPosition.z);
    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
    color += smoothstep(.15,.2, noise(st*10));
    color += smoothstep(.35,.4, noise(st*10));
    gl_FragColor = vec4(vec3(noise(st*20)), 1.0);
}
