#version 330 core
out float FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

const int kernelSize = 32;
uniform vec3 samples[kernelSize];

// uniform vec4 directions[256];

const int points = 16;
const float ssaoRadius = 5.0;
const float steps = 4.0;

void main()
{
    vec3 fragPos = texture(gPosition, TexCoords).xyz;
    vec3 normal = normalize(texture(gNormal, TexCoords).rgb);
    if (normal == vec3(0.0f)) discard;

    // Pick a random vector
    int n = (int(TexCoords.x * 7123.2315 + 125.232) *
             int(TexCoords.y * 3137.1519 + 234.8) ^
             int(fragPos.z));
    // vec4 randomVector = randomVectors[n & 255];

    // Pick a random radius
    float radius = (n + 3.0) * 0.1;
    // float radius = 0.5;
    float occlusion = 0.0;

    for (int i = 0; i < points; i++)
    {
        // Get direction
        vec3 direction = samples[i];
        // Put it in the right hemisphere
        if (dot(normal, direction) < 0.0) direction = -direction;

        float dist = 0.0;
        float depth = fragPos.z;
        // float sampleDepth = texture(gPosition, TexCoords + direction.xy * dist).z;
        // occlusion += (depth > sampleDepth ? 1.0 : 0.0);
        for (int j = 0; j < int(steps); j++)
        {
            // Step in the right direction
            dist += radius;
            depth -= direction.z * dist;

            // Read the depth from the sample
            float sampleDepth = texture(gPosition, TexCoords + direction.xy * dist).z;

            // Calculate the sample's contribution to the occlusion
            float contribution = abs(sampleDepth - fragPos.z);
            contribution *= contribution;

            // Accumulate occlusion
            if ((depth - sampleDepth) > 0.0)
                occlusion += steps / (1.0 + contribution);
        }
    }

    FragColor = 1.0 - occlusion / (steps * points);
    // FragColor = (fragPos.z + 20)/20;
}
