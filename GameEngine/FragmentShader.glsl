// Fragment Shader source code
const char* fragmentShaderSource = R"(
    #version 330 core

    struct sMaterial {
        float shininess;
        vec3 diffuse;
        vec3 specular;
    };

    struct sLight {
        vec3 lightPosition;
        vec4 lightColor;
    };

    in vec3 FragPos;    // Incoming fragment position from vertex shader
    in vec3 FragNormal; // Incoming fragment normal from vertex shader

    out vec4 FragColor;

    uniform sMaterial material;
    uniform sLight lights[10];  // Assuming you have a maximum of 10 lights
    uniform int numberOfLights; // Pass the number of active lights

    uniform vec3 camLocation;   // Camera position

    void main() {
        vec3 norm = normalize(FragNormal);
        vec3 viewDir = normalize(camLocation - FragPos);

        vec3 ambient = vec3(0.1) * vec3(lights[0].lightColor); // Global ambient light
        vec3 diffuse = vec3(0.0);
        vec3 specular = vec3(0.0);

        // Loop over all the lights
        for (int i = 0; i < numberOfLights; i++) {
            // Diffuse
            vec3 lightDir = normalize(lights[i].lightPosition - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            diffuse += diff * material.diffuse * vec3(lights[i].lightColor);

            // Specular
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            specular += spec * material.specular * vec3(lights[i].lightColor);
        }

        // Final color
        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);
    }
)";