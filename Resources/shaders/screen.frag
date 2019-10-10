#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColorSpec;
uniform sampler2D ssao;

uniform mat4 view;

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

void main()
{
    const DirectionalLight light = DirectionalLight(
        vec3(5.2f, -1.0f, 1.3f),
        vec3(0.5f, 0.5f, 0.5f),
        vec3(0.4f, 0.4f, 0.4f),
        vec3(0.8f, 0.8f, 0.8f)
    );
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gColorSpec, TexCoords).rgb;
    float Specular = texture(gNormal, TexCoords).a;
    float AmbientOcclusion = texture(ssao, TexCoords).r;

    vec3 lightDir = (view * vec4(normalize(-light.direction), 0.0f)).xyz;
    vec3 viewDir = normalize(-FragPos);

    vec3 ambient = light.ambient * AmbientOcclusion;
    vec3 diffuse = max(dot(Normal, lightDir), 0.0) * light.diffuse;
    vec3 halfwayDir = reflect(-lightDir, Normal);
    vec3 specular = pow(max(dot(viewDir, halfwayDir), 0.0), 32) * light.specular;
    vec3 lighting = Albedo * (ambient + diffuse + specular);

    FragColor = vec4(vec3(lighting), 1.0);
}
