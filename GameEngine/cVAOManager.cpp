#include "cVAOManager.h"
#include "cShaderCompiler.h"

GLuint cVAOManager::BindVAOVBO(GLuint &VBO, int numberOfMeshesToLoad, std::vector<cLoadModels> pModels) {
    cShaderCompiler shader;
    shaderProgram = shader.CompileShader();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s)
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    const GLint vertexPosition_location = glGetAttribLocation(shaderProgram, "aPos");
    const GLint vertexNormal_location = glGetAttribLocation(shaderProgram, "vNormal");
    const GLint vertexColor_location = glGetAttribLocation(shaderProgram, "aCol");

    glBindVertexArray(VAO);

    glEnableVertexAttribArray(vertexPosition_location);
    glVertexAttribPointer(vertexPosition_location,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(cLoadModels::sVerticesToRender),
        (void*)offsetof(cLoadModels::sVerticesToRender, cLoadModels::sVerticesToRender::vertexPosition));

    glEnableVertexAttribArray(vertexNormal_location);
    glVertexAttribPointer(vertexNormal_location,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(cLoadModels::sVerticesToRender),
        (void*)offsetof(cLoadModels::sVerticesToRender, cLoadModels::sVerticesToRender::vertexNormal));

    glEnableVertexAttribArray(vertexColor_location);
    glVertexAttribPointer(vertexColor_location,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(cLoadModels::sVerticesToRender),
        (void*)offsetof(cLoadModels::sVerticesToRender, cLoadModels::sVerticesToRender::vertexColor));

    int numberOfVerticesToRenderForAllModels = 0;
    for (int index = 0; index != numberOfMeshesToLoad; index++) {
        numberOfVerticesToRenderForAllModels += pModels[index].numberOfVerticesToRender;
    }

    int size_of_VAO = sizeof(cLoadModels::sVerticesToRender) * numberOfVerticesToRenderForAllModels;
    glBufferData(GL_ARRAY_BUFFER, size_of_VAO, nullptr, GL_STATIC_DRAW);

    int offset = 0;
    for (int index = 0; index != pModels.size(); index++) {
        int size_of_VAO_for_model = sizeof(cLoadModels::sVerticesToRender) * pModels[index].numberOfVerticesToRender;
        glBufferSubData(GL_ARRAY_BUFFER, offset, size_of_VAO_for_model, pModels[index].pVerticesToRender);
        offset += size_of_VAO_for_model;  // Move the offset to the next position
    }
    
    // Unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

void cVAOManager::VAOVBOCleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}
