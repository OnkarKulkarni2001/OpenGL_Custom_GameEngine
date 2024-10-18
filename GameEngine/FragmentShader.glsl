// Fragment Shader source code
const char* fragmentShaderSource = R"(
    #version 330 core
    in vec3 FragPos;    // Incoming fragment position from vertex shader
    in vec3 FragNormal; // Incoming fragment normal from vertex shader
    in vec4 FragCol;    // Incoming color from vertex shader

    out vec4 FragColor;

    void main() {
        // Use the color passed from the vertex shader
        FragColor = FragCol; // Output the interpolated color
    }
)";
