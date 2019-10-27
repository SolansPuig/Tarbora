#version 330 core
out vec4 FragColor;

uniform sampler2D scene;

in vec2 TexCoords;

void main()
{
    vec4 color = texture(scene, TexCoords);
    if (color.a == 0.0){
        discard;
    }
    FragColor = color;
}
