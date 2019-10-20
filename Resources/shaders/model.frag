#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColorSpec;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D albedo;
uniform sampler2D specular;

void main()
{
    gPosition = FragPos;
    gNormal = normalize(Normal);
    vec4 fragTexture = texture(albedo, TexCoord);
    vec4 specularTexture = texture(specular, TexCoord);
    if (fragTexture.a == 0.0){
        discard;
    }
    gColorSpec.rgb = fragTexture.rgb;
    gColorSpec.a = specularTexture.r;
}
