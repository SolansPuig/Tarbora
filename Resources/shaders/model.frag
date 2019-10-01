#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColorSpec;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D myTexture;

void main()
{
    gPosition = FragPos;
    gNormal = normalize(Normal);
    vec4 fragTexture = texture(myTexture, TexCoord);
    if (fragTexture.a == 0.0){
        discard;
    }
    gColorSpec.rgb = fragTexture.rgb;
    gColorSpec.a = 1.0f;
}
