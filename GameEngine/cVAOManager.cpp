#include "cVAOManager.h"
#include "cShaderCompiler.h"

//GLuint cVAOManager::BindVAOVBO(GLuint &VBO, int numberOfMeshesToLoad, std::vector<cLoadModels> pModels) {
//    cShaderCompiler shader;
//    shaderProgram = shader.CompileShader();
//
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//
//    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s)
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//    const GLint vertexPosition_location = glGetAttribLocation(shaderProgram, "aPos");
//    const GLint vertexNormal_location = glGetAttribLocation(shaderProgram, "vNormal");
//    const GLint vertexColor_location = glGetAttribLocation(shaderProgram, "aCol");
//
//    glEnableVertexAttribArray(vertexPosition_location);
//    glVertexAttribPointer(vertexPosition_location,
//        3,
//        GL_FLOAT,
//        GL_FALSE,
//        sizeof(cLoadModels::sVerticesToRender),
//        (void*)offsetof(cLoadModels::sVerticesToRender, cLoadModels::sVerticesToRender::vertexPosition));
//
//    glEnableVertexAttribArray(vertexNormal_location);
//    glVertexAttribPointer(vertexNormal_location,
//        3,
//        GL_FLOAT,
//        GL_FALSE,
//        sizeof(cLoadModels::sVerticesToRender),
//        (void*)offsetof(cLoadModels::sVerticesToRender, cLoadModels::sVerticesToRender::vertexNormal));
//
//    glEnableVertexAttribArray(vertexColor_location);
//    glVertexAttribPointer(vertexColor_location,
//        4,
//        GL_FLOAT,
//        GL_FALSE,
//        sizeof(cLoadModels::sVerticesToRender),
//        (void*)offsetof(cLoadModels::sVerticesToRender, cLoadModels::sVerticesToRender::vertexColor));
//
//    //int numberOfVerticesToRenderForAllModels = 0;
//    //for (int index = 0; index != numberOfMeshesToLoad; index++) {
//    //    numberOfVerticesToRenderForAllModels += pModels[index].numberOfVerticesToRender;
//    //}
//
//    int size_of_VAO = sizeof(cLoadModels::sVerticesToRender) * numberOfVerticesToRenderForAllModels;
//    glBufferData(GL_ARRAY_BUFFER, size_of_VAO, nullptr, GL_STATIC_DRAW);
//
//    int offset = 0;
//    for (int index = 0; index != pModels.size(); index++) {
//        int size_of_VAO_for_model = sizeof(cLoadModels::sVerticesToRender) * pModels[index].numberOfVerticesToRender;
//        glBufferSubData(GL_ARRAY_BUFFER, offset, size_of_VAO_for_model, pModels[index].pVerticesToRender);
//        offset += size_of_VAO_for_model;  // Move the offset to the next position
//    }
//    
//    // Unbind the VBO and VAO
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//
//    return VAO;
//}

void cVAOManager::GettingModelReadyToRender(cLoadModels& model)
{
    cShaderCompiler shader;
    shaderProgram = shader.CompileShader();

    glGenVertexArrays(1, &(model.VAO_ID));
    glBindVertexArray(model.VAO_ID);

    glGenBuffers(1, &(model.VBO_ID));
    glBindBuffer(GL_ARRAY_BUFFER, model.VBO_ID);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(cLoadModels::sVertex) * model.numberOfVertices,
                 (GLvoid*)model.pVertex,
                 GL_STATIC_DRAW);

    glGenBuffers(1, &(model.IBO_ID));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.IBO_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(unsigned int) * model.numberOfIndices,
                 (GLvoid*)model.pIndices,
                 GL_STATIC_DRAW);


    GLint vpos_location = glGetAttribLocation(shaderProgram, "aPos");
    GLint vnorm_location = glGetAttribLocation(shaderProgram, "vNormal");
    GLint vcol_location = glGetAttribLocation(shaderProgram, "aCol");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location,
                          3,
                          GL_FLOAT, GL_FALSE,
                          sizeof(cLoadModels::sVertex),
                          (void*)offsetof(cLoadModels::sVertex, x));

    glEnableVertexAttribArray(vnorm_location);
    glVertexAttribPointer(vnorm_location,
        3,
        GL_FLOAT, GL_FALSE,
        sizeof(cLoadModels::sVertex),
        (void*)offsetof(cLoadModels::sVertex, nx));

    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location,
        4,
        GL_FLOAT, GL_FALSE,
        sizeof(cLoadModels::sVertex),
        (void*)offsetof(cLoadModels::sVertex, r));

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(vpos_location);
    glDisableVertexAttribArray(vnorm_location);
    glDisableVertexAttribArray(vcol_location);

}

void cVAOManager::VAOVBOCleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}
