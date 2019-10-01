#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColorSpec;
uniform sampler2D ssao;

uniform vec3 viewPos;

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

void main()
{
    const DirectionalLight light = DirectionalLight(
        vec3(1.2f, -1.0f, 1.3f),
        vec3(0.7f, 0.7f, 0.7f),
        vec3(0.4f, 0.4f, 0.4f),
        vec3(1.0f, 1.0f, 1.0f)
    );
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gColorSpec, TexCoords).rgb;
    float Specular = texture(gNormal, TexCoords).a;
    float AmbientOcclusion = texture(ssao, TexCoords).r;

    vec3 lightDir = normalize(-light.direction);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 ambient = light.ambient * AmbientOcclusion;
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse;
    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * spec;
    vec3 lighting = Albedo * (ambient + diffuse + specular);

    FragColor = vec4(vec3(lighting), 1.0);
}
