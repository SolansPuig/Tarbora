#version 330 core
out vec4 FragColor;

uniform sampler2D tex;

in vec2 TexCoords;

void main()
{
    vec4 color = texture(tex, TexCoords);
    if (color.a == 0.0)
    {
      discard;
    }
    FragColor = color;
}
