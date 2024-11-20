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
#include "cLightManager.h"
#include "cLightMover.h"
#include "cPhysicsUpdated.h"
#include "cRenderModel.h"
#include "cLua.h"
#include "cTextureCreator.h"

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
    glUseProgram(shaderProgram);

    // Import Scene
    cScene scene;
    scene.CreateScene("../sceneFileWithNormals.txt");
    //scene.ExportMaterial(shaderProgram, scene.numberOfMeshesToLoad);            // Considering number of materials = number of meshes to load

    cVAOManager VAOManager;
    for (int i = 0; i != scene.pModels.size(); i++) {
        VAOManager.GettingModelReadyToRender(scene.pModels[i]);         // This thing is new just because I created whole new VAO thing which creates several different VAOs and now I can render a single model multiple times
    }

    cRenderModel renderer;

    /*int numberOfVerticesToRenderForAllModels = 0;
    for (int index = 0; index != scene.pModels.size(); index++) {
        numberOfVerticesToRenderForAllModels += scene.pModels[index].numberOfVerticesToRender;
    }*/

    cLightManager lightManager;
    lightManager.LoadLights("../lightsFile.txt");
    // Camera Initialization
    FlyCam flyCam(800, 600, glm::vec3(0.0, 0.0, 0.0), 180.0f);
    //flyCam.camSpeed = 1.0f;

    cLightMover lightMover(lightManager, flyCam, 1);


    float deltaTime = 0;
    float startTime, endTime;

    for (int modelIndex = 0; modelIndex != scene.numberOfMeshesToLoad; modelIndex++) {
        scene.pModels[modelIndex].pTransformedVertices = new cLoadModels::sTransformedVertex[scene.pModels[modelIndex].numberOfVertices];
    
        glm::mat4 model = scene.pModels[modelIndex].CreateModelMatrix(shaderProgram, scene.pModels[modelIndex]);      // Creation of model matrix with arguements passed in sceneFile.txt
        scene.pModels[modelIndex].GenerateTransformedVertices(model);
    }   // Used for initializing the pTransformedVertices, Nothing new xD

    // Starting physics
    cPhysicsUpdated physicsEngine(scene);

    startTime = glfwGetTime();
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // ------------------------------------------Texture Testing---------------------------------------
    cTextureCreator textureCreator;
    textureCreator.LoadTextures(shaderProgram, "../SpaceInteriors_Texture.bmp", true);
    // ------------------------------------------Texture Testing---------------------------------------



    // -----------------------------------------------LUA----------------------------------------------
    
    cLua lua;
    lua_State* state = luaL_newstate();
    lua.InitLua(state);
    //lua.ExecuteLuaScript(state, "game_logic.lua");      // Need to implement this script game_logic.lua

    // -----------------------------------------------LUA----------------------------------------------


    // Render loop
    while (!glfwWindowShouldClose(window)) {

        // Input handling
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        glfwGetWindowSize(window, &flyCam.camWidth, &flyCam.camHeight);

        std::string cameraPositionInTitle = "Camera Location: (" + to_string(flyCam.camLocation.x) + ", " + to_string(flyCam.camLocation.y) + ", " + to_string(flyCam.camLocation.z) + ") Camera Speed: " + to_string(flyCam.camSpeed) + " Camera Roll: " + to_string(flyCam.camRoll) + " Cam yaw: " + to_string(flyCam.camYaw) + " Cam pitch: " + to_string(flyCam.camPitch);
        glfwSetWindowTitle(window, cameraPositionInTitle.c_str());

        endTime = glfwGetTime();
        deltaTime = endTime - startTime;
        //std::cout << deltaTime << " Time passed" << std::endl;
        
        flyCam.cameraMatrix(45.0f, 0.1f, 1000.0f, shaderProgram, "camMatrix", window);

        // Rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glEnable(GL_DEPTH_TEST);   // Enable depth testing
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader program and draw the triangle
        glUseProgram(shaderProgram);

        //lightMover.MoveForward();
        //lightMover.MoveUp();
        //lightMover.MoveRight();

        // ------------------------------------------------------------------------------------------------------------------------------
        // You can call movement functions from light mover class for lights here now and then call turn on lights function of light manager
        lightManager.TurnOnLights(shaderProgram, 1);
        // ------------------------------------------------------------------------------------------------------------------------------
        // You can create player objects here and make them move from here
        // ------------------------------------------------------------------------------------------------------------------------------

        //glBindVertexArray(VAO);
        //int offset = 0;
        //for (int index = 0; index != scene.pModels.size(); index++) {
        //
        //    glm::mat4 model = scene.pModels[index].CreateModelMatrix(shaderProgram, scene.pModels[index]);      // Creation of model matrix with arguements passed in sceneFile.txt
        //    scene.pModels[index].GenerateTransformedVertices(model);            // this is here because collisions want transformed vertices
        //
        //    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        //    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        //
        //    glDrawArrays(GL_TRIANGLES, offset, scene.pModels[index].numberOfVerticesToRender);
        //    offset += scene.pModels[index].numberOfVerticesToRender;
        //}

        for (int i = 0; i != scene.pModels.size(); i++) {
            if (scene.pModels[i].bIsWireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else {
                shader.SetSceneView(window);        // Press 1, 2 or 3 for different viewModes like wireframe, fill or point
            }
            renderer.Render(shaderProgram, &scene.pModels[i]);
        }

        //renderer.DrawDebugSphere(&scene.pModels[0], glm::vec3(0, 0, 0), glm::vec4(0, 1, 0, 1), 1, shaderProgram);
        
        //if (deltaTime >= 10) {
            /*if (physicsEngine.CheckCollision(scene)) {
                std::cout << "Colliding" << std::endl;
            }
            else {
                std::cout << "Not colliding" << std::endl;
            }*/
            //startTime = endTime;
        //}
            //dragon.SetSpeed(0.1f);
        

        // Swap buffers and poll IO events (keys pressed/released, mouse moved, etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    // -----------------------------------------------LUA----------------------------------------------
    
    lua_close(state);   // Lua cleanup

    // -----------------------------------------------LUA----------------------------------------------



    // Cleanup
    VAOManager.VAOVBOCleanup();
    
    // Terminate GLFW
    glfwTerminate();
    return 0;
}