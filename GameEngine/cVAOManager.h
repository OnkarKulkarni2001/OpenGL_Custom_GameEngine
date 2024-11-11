#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "cLoadModels.h"
#include "cScene.h"

class cVAOManager {
public:
    GLuint VAO, VBO;
    GLuint shaderProgram;
    int size_of_VAO;

    void GettingModelReadyToRender(cLoadModels& model);

    void VAOVBOCleanup();
};