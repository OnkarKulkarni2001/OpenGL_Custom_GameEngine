// Vertex Shader source code
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    uniform mat4 model; // Model matrix
    uniform mat4 camMatrix;
    
    out vec3 FragPos; // Pass the position to the fragment shader
    void main() {
       gl_Position = camMatrix * model * vec4(aPos, 1.0);
    }
)";