#version 330 core

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform vec3 highlightColor; // base object color (RGB)
uniform vec3 baseColor;
uniform float startTime = -1;
uniform float time = -1;

out vec4 FragColor;

void main()
{
    // === Lighting setup ===
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos);
    vec3 viewDir = normalize(cameraPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float progress = 0.0;
    vec3 ourColor = baseColor;

    if(FragPos.z <= -10)
        ourColor = vec3(0.11, 0.243, 0.89);

    // if(FragPos.z > -30 && FragPos.z <= 0.0)
    //     ourColor = vec3(0.0, 0.827, 1.0);

    if(FragPos.z > -10 && FragPos.z <= 30.0)
        ourColor = vec3(0.0, 1.0, 0.4);

    if(FragPos.z > 30 && FragPos.z <= 45)
        ourColor = vec3(0.231, 0.769, 0.537);

    if(FragPos.z > 45)
        ourColor = vec3(0.671, 0.365, 0.329);

    // if(FragPos.z > 45)
    //     ourColor = vec3(0.827, 0.827, 0.827);

    // Ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * vec3(ourColor);

    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(ourColor);

    // Specular
    float specularStrength = 0.4;
    float shininess = 32.0;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * vec3(1.0); // white specular

    // Final color
    vec3 result = ambient + diffuse;
    FragColor = vec4(result, 1.0);
}
