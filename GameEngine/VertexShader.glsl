// Vertex Shader source code
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 vNormal;

    uniform mat4 model; // Model matrix
    uniform mat4 camMatrix;
    
    out vec3 FragPos; // Pass the position to the fragment shader
    out vec3 FragNormal;

    void main() {
       FragPos = vec3(model * vec4(aPos, 1.0));         // Transform position to world space
       FragNormal = mat3(transpose(inverse(model))) * vNormal;  // Transform normal to world space
       gl_Position = camMatrix * vec4(FragPos, 1.0);    // Compute final position
    }
)";