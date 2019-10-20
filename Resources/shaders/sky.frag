#version 330 core
out vec4 FragColor;

uniform sampler2D albedo;
uniform sampler2D specular;

in vec2 TexCoord;

void main()
{
    vec4 fragTexture = texture(albedo, TexCoord);
    if (fragTexture.a == 0.0){
        discard;
    }

    FragColor = fragTexture;
}
