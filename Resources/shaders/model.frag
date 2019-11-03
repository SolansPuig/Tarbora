#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColorSpec;

in vec2 TexCoord;
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
    gPosition = FragPos;
    gNormal = normalize(Normal);
    vec4 fragTexture = texture(albedo, TexCoord);
    vec4 specularTexture = texture(specular, TexCoord);
    vec4 colorTintTexture = texture(colorTint, TexCoord);
    if (fragTexture.a == 0.0){
        discard;
    }

    vec3 primary = colorPrimary * colorTintTexture.r + (1. - colorTintTexture.r);
    vec3 secondary = colorSecondary * colorTintTexture.g + (1. - colorTintTexture.g);
    vec3 detail = colorDetail * colorTintTexture.b + (1. - colorTintTexture.b);
    vec3 detail2 = colorDetail2 * colorTintTexture.a + (1. - colorTintTexture.a);

    gColorSpec.rgb = clamp(fragTexture.rgb * primary * secondary * detail * detail2, 0.0, 1.0);
    // gColorSpec.rgb = colorTintTexture.rgb;
    gColorSpec.a = specularTexture.r;
}
