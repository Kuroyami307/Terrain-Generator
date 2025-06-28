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

    vec3 tipColor = vec3(0.5, 0.8, 0.2);
    vec3 basColor = vec3(0.1, 0.8, 0.2);

    vec3 ourColor = mix(basColor, tipColor, FragPos.z / 15);
    // if(FragPos.z <= -180)
    //     ourColor = vec3(0.0f, 0.3f, 0.8f);

    // if(FragPos.z > -180 && FragPos.z <= -100)
    //     ourColor = vec3(0.6f, 0.5f, 0.4f);

    // if(FragPos.z > -100)
    //     ourColor = vec3(0.1f, 0.8f, 0.2f);



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
