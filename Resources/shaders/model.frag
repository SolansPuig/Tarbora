#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColorSpec;

in vec2 TexPos;
in vec2 TexSize;
in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec3 colorPrimary;
in vec3 colorSecondary;
in vec3 colorDetail;
in vec3 colorDetail2;

uniform sampler2D albedo;
uniform sampler2D specular;
uniform sampler2D colorTint;


void main()
{
    vec2 texCoord = TexPos + mod(TexCoords, TexSize);
    gPosition = FragPos;
    gNormal = normalize(Normal);
    vec4 fragTexture = texture(albedo, texCoord);
    vec4 specularTexture = texture(specular, texCoord);
    vec4 mask = texture(colorTint, texCoord);
    if (fragTexture.a == 0.0){
        discard;
    }

    vec3 primary = colorPrimary * mask.r + (1. - mask.r);
    vec3 secondary = colorSecondary * mask.g + (1. - mask.g);
    vec3 detail = colorDetail * mask.b + (1. - mask.b);
    vec3 detail2 = colorDetail2 * mask.a + (1. - mask.a);

    gColorSpec.rgb = fragTexture.rgb * primary * secondary * detail * detail2;
    gColorSpec.a = specularTexture.r;
}
