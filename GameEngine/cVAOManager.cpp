#include "cVAOManager.h"
#include "cShaderCompiler.h"

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
