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
#include <sstream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

float getRandomFloat(float a, float b) {
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

glm::vec3 getRandom_vec3(glm::vec3 min, glm::vec3 max)
{
    return glm::vec3(
        getRandomFloat(min.x, max.x),
        getRandomFloat(min.y, max.y),
        getRandomFloat(min.z, max.z));
}

std::string getStringVec3(glm::vec3 theVec3)
{
    std::stringstream ssVec;
    ssVec << "(" << theVec3.x << ", " << theVec3.y << ", " << theVec3.z << ")";
    return ssVec.str();
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set OpenGL version (3.3 core profile)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);                  // glTexStorage2D is not supported on version 3, need to use 4!!!
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
    FlyCam flyCam(800, 600, glm::vec3(347.0, 174.0, -19.0), 180.0f);
    flyCam.camSpeed = 10.0f;
    flyCam.camPitch = -22.0f;

    cLightMover lightMover(lightManager, flyCam, 1);


    float deltaTime = 0;
    float startTime, endTime;

    for (int modelIndex = 0; modelIndex != scene.numberOfMeshesToLoad; modelIndex++) {
        scene.pModels[modelIndex].pTransformedVertices = new cLoadModels::sTransformedVertex[scene.pModels[modelIndex].numberOfVertices];
    
        glm::mat4 model = scene.pModels[modelIndex].CreateModelMatrix(shaderProgram, scene.pModels[modelIndex]);      // Creation of model matrix with arguements passed in sceneFile.txt
        scene.pModels[modelIndex].GenerateTransformedVertices(model);
    }   // Used for initializing the pTransformedVertices, Nothing new xD

    // Starting physics
   // cPhysicsUpdated physicsEngine(scene);

    startTime = glfwGetTime();
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);




    // ------------------------------------------Texture---------------------------------------

    cTextureCreator textureCreator;

    for (int modelIndex = 0; modelIndex != scene.numberOfMeshesToLoad; modelIndex++) {
        if (scene.pModels[modelIndex].bIsCubeMap == false) {
            textureCreator.LoadTextures24Bit(shaderProgram, scene.pModels[modelIndex], true);
        }
        else {
            textureCreator.LoadCubeMap24Bit(shaderProgram, true, scene.pModels[modelIndex].cubeMapTextureID,
                scene.pModels[modelIndex].textureFilePaths[0], scene.pModels[modelIndex].textureFilePaths[1],
                scene.pModels[modelIndex].textureFilePaths[2], scene.pModels[modelIndex].textureFilePaths[3],
                scene.pModels[modelIndex].textureFilePaths[4], scene.pModels[modelIndex].textureFilePaths[5]);
            //textureCreator.LoadCubeMap24Bit(shaderProgram, true, scene.pModels[modelIndex].cubeMapTextureID,
            //    "../assets/CubeMaps/TropicalSunnyDayUp2048.bmp",
            //    "../assets/CubeMaps/TropicalSunnyDayBack2048.bmp",
            //    "../assets/CubeMaps/TropicalSunnyDayDown2048.bmp",
            //    "../assets/CubeMaps/TropicalSunnyDayFront2048.bmp",
            //    "../assets/CubeMaps/TropicalSunnyDayLeft2048.bmp",
            //    "../assets/CubeMaps/TropicalSunnyDayRight2048.bmp");
        }
    }

    glEnable(GL_BLEND);                                 // For transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ------------------------------------------Texture---------------------------------------

        // ------------------------------Multiple Bunnies----------------------------------------------------
    float boxLimit = 100.0f;
    float boxStep = 20.0f;
    unsigned int ID_count = 0;
    for (float x = -boxLimit; x <= boxLimit; x += boxStep)
    {
        for (float z = -(2.0f * boxLimit); z <= boxLimit; z += boxStep)
        {
            cLoadModels* pBunny = new cLoadModels();
            //            pBunny->modelFileName = "assets/models/bun_zipper_res2_10x_size_xyz_only.ply";
            //            pBunny->modelFileName = "assets/models/bun_zipper_res2_10x_size_xyz_N_only.ply";
            pBunny = &scene.pModels[1];
            pBunny->pMeshTransform.x = x;
            pBunny->pMeshTransform.y = 30.0f;
            pBunny->pMeshTransform.z = z;

            pBunny->numberOfTextures = 1;
            pBunny->textureFilePaths = scene.pModels[1].textureFilePaths;
            pBunny->textures.resize(scene.pModels[1].numberOfTextures);
            pBunny->textures = scene.pModels[1].textures;
            // Set some transparency
            pBunny->transparencyIndex = getRandomFloat(0.25f, 1.0f);
            pBunny->bIsTransparent = true;

            scene.pModels.push_back(*pBunny);
        }
    }//for (float x = -boxLimit...
    // ------------------------------Multiple Bunnies----------------------------------------------------

    scene.pModels[0].bIsReflective = true;
    scene.pModels[0].reflectiveIndex = 1;

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
        
        flyCam.cameraMatrix(45.0f, 0.1f, 10000.0f, shaderProgram, "camMatrix", window);

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

        // Cube map object position should go with camera (scene.pModels[2] is cubeMapObject)
        for (int i = 0; i != scene.pModels.size(); i++) {
            if (scene.pModels[i].bIsCubeMap) {
                scene.pModels[i].pMeshTransform.x = flyCam.camLocation.x;
                scene.pModels[i].pMeshTransform.y = flyCam.camLocation.y;
                scene.pModels[i].pMeshTransform.z = flyCam.camLocation.z;
            }
        }
        
        for (int i = 0; i != scene.pModels.size()-1; i++) {
            if (scene.pModels[i].bIsWireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else {
                shader.SetSceneView(window);        // Press 1, 2 or 3 for different viewModes like wireframe, fill or point
            }
            renderer.Render(shaderProgram, &scene.pModels[i]);
        }

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