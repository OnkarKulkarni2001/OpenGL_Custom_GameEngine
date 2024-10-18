#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "cLoadModels.h"
#include "FlyCam.h"
#include "cShaderCompiler.h"
#include "cVAOManager.h"
#include "cScene.h"
#include "IPlayer.h"
#include "cPlayer.h"
#include "cAiEnemy.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set OpenGL version (3.3 core profile)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Triangle", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Refer cShaderCompiler class for more info.
    cShaderCompiler shader;
    GLuint shaderProgram = shader.CompileShader();

    // Import Scene
    cScene scene;
    scene.CreateScene();
    
    GLuint VAO, VBO;
    cVAOManager VAOManager;
    VAO = VAOManager.BindVAOVBO(VBO, scene.numberOfMeshesToLoad, scene.pModels);
    
    int numberOfVerticesToRenderForAllModels = 0;
    for (int index = 0; index != scene.pModels.size(); index++) {
        numberOfVerticesToRenderForAllModels += scene.pModels[index].numberOfVerticesToRender;
    }


    // Camera Initialization
    FlyCam flyCam(800, 600, glm::vec3(0.0f, 0.0f, -4.0f), 180.0f);
    
    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Input handling
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        glfwGetWindowSize(window, &flyCam.camWidth, &flyCam.camHeight);

        //flyCam.camControls(window);
        flyCam.cameraMatrix(45.0f, 0.1f, 1000.0f, shaderProgram, "camMatrix", window);

        // Rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use our shader program and draw the triangle
        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        int offset = 0;
        for (int index = 0; index != scene.pModels.size(); index++) {

            glm::mat4 model = scene.pModels[index].CreateModelMatrix(shaderProgram, scene.pModels[index]);      // Creation of model matrix with arguements passed in sceneFile.txt
            unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, offset, scene.pModels[index].numberOfVerticesToRender);

            offset += scene.pModels[index].numberOfVerticesToRender;
        }

        // Swap buffers and poll IO events (keys pressed/released, mouse moved, etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    VAOManager.VAOVBOCleanup();
    
    // Terminate GLFW
    glfwTerminate();
    return 0;
}