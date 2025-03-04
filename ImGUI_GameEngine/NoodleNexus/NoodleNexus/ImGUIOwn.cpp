#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

//#include <GLFW/glfw3.h>
#include "sMesh.h"
#include "cVAOManager/cVAOManager.h"
#include "cLightManager.h"
#include <string>
#include <cstring>
#include <iostream>

sMesh* g_pFindMeshByFriendlyName(std::string theNameToFind);
sMesh* myMesh = nullptr;
extern std::vector<sMesh*> g_vecMeshesToDraw;
extern cVAOManager* g_pMeshManager;
extern GLuint program;
extern GLFWwindow* window;
extern cLightManager* g_pLightManager;
extern bool g_bShowDebugSpheres;
extern unsigned int g_selectedLightIndex;

static char modelFilePath[256] = "assets/models/.ply";
static char friendlyName[128] = "";
static glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
static glm::vec3 rotation = glm::vec3(0.0f);
static char textureFile[128] = ".bmp";
static float blendRatio = 1.0f;
static float scale = 1.0f;
static bool doNotLight = false;
sModelDrawInfo meshInfo;

void RenderImGui()
{
    static char meshName[128] = "";
    static bool dockspaceOpen = true;

    // Get current window size
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    // Set next window position and size (dock to right)
    ImGui::SetNextWindowPos(ImVec2(width * 0.8f, 0)); // Right-side dock (70% OpenGL space)
    ImGui::SetNextWindowSize(ImVec2(width * 0.2f, height)); // 30% width, full height
    //ImGui::SetNextWindowSize(ImVec2(350, 500), ImGuiCond_FirstUseEver);

    ImGui::Begin("Mesh Manager", &dockspaceOpen, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::BeginTabBar("MeshTabs"))
    {
        // First tab - Add New Mesh
        if (ImGui::BeginTabItem("Add New Mesh"))
        {
            ImGui::InputText("Model File", modelFilePath, IM_ARRAYSIZE(modelFilePath));
            ImGui::InputText("Unique Name", friendlyName, IM_ARRAYSIZE(friendlyName));
            ImGui::DragFloat3("Position", &position[0], 1.0f);
            ImGui::DragFloat3("Rotation", &rotation[0], 1.0f);
            ImGui::InputText("Texture", textureFile, IM_ARRAYSIZE(textureFile));
            ImGui::SliderFloat("Blend Ratio", &blendRatio, 0.0f, 1.0f);
            ImGui::DragFloat("Scale", &scale, 0.1f, 0.01f, 50.0f);
            ImGui::Checkbox("No Lighting", &doNotLight);

            if (ImGui::Button("Add Mesh"))
            {
                ::g_pMeshManager->LoadModelIntoVAO_Async(modelFilePath, meshInfo, program);
                sMesh* newMesh = new sMesh();
                newMesh->modelFileName = modelFilePath;
                newMesh->uniqueFriendlyName = friendlyName;
                newMesh->positionXYZ = position;
                newMesh->rotationEulerXYZ = rotation;
                newMesh->textures[0] = textureFile;
                newMesh->blendRatio[0] = blendRatio;
                newMesh->uniformScale = scale;
                newMesh->bDoNotLight = doNotLight;

                ::g_vecMeshesToDraw.push_back(newMesh);
                std::cout << "Mesh " << friendlyName << " added to scene.\n";
            }

            ImGui::EndTabItem();
        }

        // Second tab - Mesh Editor
        if (ImGui::BeginTabItem("Mesh Editor"))
        {
            ImGui::InputText("Mesh Name", meshName, IM_ARRAYSIZE(meshName));
            if (ImGui::Button("Find Mesh")) {
                myMesh = g_pFindMeshByFriendlyName(meshName);
            }

            if (myMesh) {
                ImGui::DragFloat3("Position", &myMesh->positionXYZ[0], 0.1f);
                ImGui::DragFloat3("Rotation", &myMesh->rotationEulerXYZ[0], 1.0f);
                ImGui::DragFloat("Scale", &myMesh->uniformScale, 0.01f, 0.01f, 10.0f);
                ImGui::ColorEdit4("Color RGBA", &myMesh->objectColourRGBA[0]);
                ImGui::Checkbox("Override Color", &myMesh->bOverrideObjectColour);
                ImGui::Checkbox("Wireframe", &myMesh->bIsWireframe);
                ImGui::Checkbox("Visible", &myMesh->bIsVisible);
                ImGui::Checkbox("No Lighting", &myMesh->bDoNotLight);
                ImGui::DragFloat("Transparency", &myMesh->alphaTransparency, 0.01f, 0.0f, 1.0f);
            }

            ImGui::EndTabItem();
        }

        //if (ImGui::BeginTabItem("Lights"))
        //{
        //    static int selectedLight = 0; // Stores the index of the current light

        //    // Select which light to edit
        //    ImGui::Text("Select Light:");
        //    ImGui::SliderInt("Light Index", &selectedLight, 0, cLightManager::NUMBEROFLIGHTS - 1);

        //    // Get reference to the selected light
        //    cLightManager::sLight& light = ::g_pLightManager->theLights[selectedLight];

        //    // Light Position
        //    ImGui::SliderFloat3("Position", &light.position.x, -100000.0f, 100000.0f);

        //    // Light Color
        //    ImGui::ColorEdit3("Diffuse Color", &light.diffuse.x);

        //    // Attenuation
        //    ImGui::SliderFloat("Linear Attenuation", &light.atten.y, 0.00000000001f, 0.01f, "%.009f");
        //    ImGui::SliderFloat("Quadratic Attenuation", &light.atten.z, 0.00000000001f, 0.000000001f, "%.100f");

        //    // Light Type (Point Light, Spotlight, Directional)
        //    const char* lightTypes[] = { "Point Light", "Spotlight", "Directional" };
        //    int currentLightType = static_cast<int>(light.param1.x);

        //    if (ImGui::Combo("Light Type", &currentLightType, lightTypes, IM_ARRAYSIZE(lightTypes))) {
        //        light.param1.x = static_cast<float>(currentLightType);
        //    }

        //    // Light Toggle (On/Off)
        //    bool isLightOn = (light.param2.x > 0.5f);
        //    if (ImGui::Checkbox("Light On", &isLightOn)) {
        //        light.param2.x = isLightOn ? 1.0f : 0.0f;
        //    }

        //    ImGui::EndTabItem();
        //}

        //ImGui::EndTabBar();
        if (ImGui::BeginTabItem("Lights"))
        {
            static int selectedLight = 0; // Stores the index of the current light
            ::g_selectedLightIndex = selectedLight;

            // Select which light to edit
            ImGui::Text("Select Light:");
            ImGui::SliderInt("Light Index", &selectedLight, 0, cLightManager::NUMBEROFLIGHTS - 1);

            // Get reference to the selected light
            cLightManager::sLight& light = ::g_pLightManager->theLights[selectedLight];

            ImGui::Separator();
            ImGui::Text("Light Properties");

            // Light Position
            ImGui::DragFloat3("Position", &light.position.x, 0.1f, -100000.0f, 100000.0f);

            // Light Color
            ImGui::ColorEdit3("Diffuse Color", &light.diffuse.x);

            // Attenuation
            ImGui::DragFloat("Linear Attenuation", &light.atten.y, 0.001f, 0.00000000001f, 1.0f, "%.009f");
            ImGui::DragFloat("Quadratic Attenuation", &light.atten.z, 0.00000001f, 0.00000000001f, 0.0001f, "%.100f");

            // Light Type (Point Light, Spotlight, Directional)
            const char* lightTypes[] = { "Point Light", "Spotlight", "Directional" };
            int currentLightType = static_cast<int>(light.param1.x);

            if (ImGui::Combo("Light Type", &currentLightType, lightTypes, IM_ARRAYSIZE(lightTypes))) {
                light.param1.x = static_cast<float>(currentLightType);
            }

            // Spotlight-Specific Controls
            if (currentLightType == 1) // Spotlight
            {
                ImGui::DragFloat3("Direction", &light.direction.x, 0.01f, -1.0f, 1.0f);
                ImGui::DragFloat("Inner Cone Angle", &light.param1.y, 0.1f, 0.0f, 90.0f);
                ImGui::DragFloat("Outer Cone Angle", &light.param1.z, 0.1f, 0.0f, 90.0f);
            }

            if (currentLightType == 2) // directional light
            {
                ImGui::DragFloat3("Direction", &light.direction.x, 0.01f, -1.0f, 1.0f);
            }

            // Light Toggle (On/Off)
            bool isLightOn = (light.param2.x > 0.5f);
            if (ImGui::Checkbox("Light On", &isLightOn)) {
                light.param2.x = isLightOn ? 1.0f : 0.0f;
            }

            ImGui::Checkbox("Show Light Borders", &::g_bShowDebugSpheres);

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();


    }

    ImGui::End();
}
