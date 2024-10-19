// Vertex Shader source code
const char* vertexShaderSource = R"(
    #version 330 core
    in vec3 aPos;
    in vec3 vNormal;
    in vec3 aCol;

    uniform mat4 model; // Model matrix
    uniform mat4 camMatrix;
    
    out vec3 FragPos; // Pass the position to the fragment shader
    out vec3 FragNormal;
    out vec3 FragCol;

    void main() {
       FragCol = aCol;
       FragPos = vec3(model * vec4(aPos, 1.0));         // Transform position to world space
       FragNormal = mat3(transpose(inverse(model))) * vNormal;  // Transform normal to world space
       gl_Position = camMatrix * vec4(FragPos, 1.0);    // Compute final position
    }
)";