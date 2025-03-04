// for non keyboard callback handling

#include "GLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <iostream>

#include "sharedThings.h"   // The camera, etc.
#include "sMesh.h"

#include "cLightManager.h"

#include <fstream>
#include <sstream>
#include "../sharedThings.h"

#include "cTerrainPathChooser.h"

#include "cViperFlagConnector.h"

// The commands
//#include "cMoveRelativeTime.h"
// Now we use the g_pCommandFactory to get all of our commands

//#include <Windows.h>

//extern cLightManager* g_pLightManager;

extern cTerrainPathChooser* g_pTerrainPathChooser;

extern cViperFlagConnector* g_pViperFlagConnector;

struct sMouseState
{
    glm::ivec2 currentPositionXY = glm::ivec2(0, 0);
    glm::ivec2 lastPositionXY = glm::ivec2(0, 0);
    bool bIsLeftMouseButtonDown = false;
};

sMouseState g_MouseState;

bool isShiftDown(GLFWwindow* window)
{
    if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) || 
        (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) )
    {
        return true;
    }
    return false;
}

bool isControlDown(GLFWwindow* window)
{
    if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) ||
        (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS))
    {
        return true;
    }
    return false;
}

bool isAltDown(GLFWwindow* window)
{
    if ((glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) ||
        (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS))
    {
        return true;
    }
    return false;
}

bool areAllModifiersUp(GLFWwindow* window)
{
    if ( isShiftDown(window) ) 
    {
        return false;
    }

    if ( isControlDown(window) ) 
    {
        return false;
    }

    if ( isAltDown(window) ) 
    {
        return false;
    }

    // Nothing is down
    return true;
}

void handleMouseAsync(GLFWwindow* window)
{

//    if (glfwRawMouseMotionSupported())
//        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);


    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

//    std::cout << "Async mouse location (x,y): " << xpos << ", " << ypos << std::endl;

    return;
}

void handleKeyboardAsync(GLFWwindow* window)
{
//    const float CAMERA_MOVE_SPEED = 100.0f;
    const float CAMERA_MOVE_SPEED = 1.0f;
//    const float CAMERA_MOVE_SPEED = 0.1f;
    const float CAMERA_TURN_SPEED = 0.1f;

    if ( isShiftDown(window) )
    {
        // Keys move the target
    }

    if (isControlDown(window) )
    {
        // Find the Light_Sphere
//        sMesh* pLightSphere = pFindMeshByFriendlyName("Light_Sphere");

        const float LIGHT_MOVE_SPEED = 0.02f;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            ::g_pLightManager->theLights[g_selectedLightIndex].position.z += LIGHT_MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            ::g_pLightManager->theLights[g_selectedLightIndex].position.z -= LIGHT_MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            ::g_pLightManager->theLights[g_selectedLightIndex].position.x -= LIGHT_MOVE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            ::g_pLightManager->theLights[g_selectedLightIndex].position.x += LIGHT_MOVE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            ::g_pLightManager->theLights[g_selectedLightIndex].position.y += LIGHT_MOVE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            ::g_pLightManager->theLights[g_selectedLightIndex].position.y -= LIGHT_MOVE_SPEED;
        }
        // Linear atten 1 & 2
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            // Increase linear (making it darker)
            ::g_pLightManager->theLights[g_selectedLightIndex].atten.y *= 0.99f;     // Down 1%
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            // Increase linear (making it darker)
            ::g_pLightManager->theLights[g_selectedLightIndex].atten.y *= 1.01f;     // Up 1%
        }
        // Quadratic atten 3 & 4
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            // Quadratic linear (making it darker)
            // This is WAY more sensitive than the linear
            //  ...so you might want to change it by 0.1% (0.999)
            ::g_pLightManager->theLights[g_selectedLightIndex].atten.z *= 0.99f;    // Down 1%
        }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            // Quadratic linear (making it darker)
            ::g_pLightManager->theLights[g_selectedLightIndex].atten.z *= 1.01f;     // Up 1%
        }

        // Angles of spot light
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        {
            ::g_pLightManager->theLights[g_selectedLightIndex].param1.y -= 0.1f;     
        }       
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        {
            ::g_pLightManager->theLights[g_selectedLightIndex].param1.y += 0.1f;     
        }              

        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        {
            ::g_pLightManager->theLights[g_selectedLightIndex].param1.z -= 0.1f;     
        }       
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        {
            ::g_pLightManager->theLights[g_selectedLightIndex].param1.z += 0.1f;     
        }          
        // HACK:Exit early
        return;
    }

    // Camera now moves when shift is down
    if (isShiftDown(window))
    {

        // Move the camera 
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            // Go forward
//            ::g_cameraEye.z -= CAMERA_MOVE_SPEED;
            ::g_pFlyCamera->moveForward( CAMERA_MOVE_SPEED );
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            // Go back
//            ::g_cameraEye.z += CAMERA_MOVE_SPEED;
            ::g_pFlyCamera->moveForward( -CAMERA_MOVE_SPEED );
        }

        if ( glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS )
        {
            // Go left
//           ::g_cameraEye.x -= CAMERA_MOVE_SPEED;
            ::g_pFlyCamera->moveLeftRight( CAMERA_MOVE_SPEED );
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            // Go right
//            ::g_cameraEye.x += CAMERA_MOVE_SPEED;
            ::g_pFlyCamera->moveLeftRight( -CAMERA_MOVE_SPEED );
        }



        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            // Go down
//            ::g_cameraEye.y -= CAMERA_MOVE_SPEED;
            ::g_pFlyCamera->moveUpDown( -CAMERA_MOVE_SPEED );
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            // Go up
//            ::g_cameraEye.y += CAMERA_MOVE_SPEED;
            ::g_pFlyCamera->moveUpDown( CAMERA_MOVE_SPEED );
        }

        // Use the arrow keys to turn and pitch the camera

        // Left pitches negative (rotate around Y a negative value)
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            ::g_pFlyCamera->rotateLeftRight_Yaw( CAMERA_TURN_SPEED );
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            ::g_pFlyCamera->rotateLeftRight_Yaw( -CAMERA_TURN_SPEED );
        }

         // Left pitches negative (rotate around Y a negative value)
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            ::g_pFlyCamera->pitchUpDown( -CAMERA_TURN_SPEED );
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            ::g_pFlyCamera->pitchUpDown( CAMERA_TURN_SPEED );
        }

    }//if (areAllModifiersUp(window)


    // This will control the player's character
    if (areAllModifiersUp(window))
    {
        cPhysics::sPhysInfo* pViperPhys = ::g_pPhysicEngine->pFindAssociateMeshByFriendlyName("New_Viper_Player");

        const float VIPER_SPEED_CHANGE = 0.1f;

        // Exists? 
        if (pViperPhys)         // or != NULL
        {
            if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            {
                if (::g_pViperFlagConnector)
                {
                    ::g_pViperFlagConnector->ReleaseFlag();
                }
            }

            // CUT THE ENGINES!!!
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            {
                pViperPhys->velocity = glm::vec3(0.0f);
            }
            // Go forward
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            {
                // Accelerate
                pViperPhys->velocity.z += VIPER_SPEED_CHANGE;
            }

            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            {
                // Accelerate
                pViperPhys->velocity.z -= VIPER_SPEED_CHANGE;
            }

            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            {
                // Accelerate
                pViperPhys->velocity.x += VIPER_SPEED_CHANGE;
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            {
                pViperPhys->velocity.x -= VIPER_SPEED_CHANGE;
            }
            if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            {
                pViperPhys->velocity.y += VIPER_SPEED_CHANGE;
            }
            if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            {
                pViperPhys->velocity.y -= VIPER_SPEED_CHANGE;
            }


        }// if (pViperPhys)

    }
        
    return;
}

// *********************************************************
// Mouse callbacks:

// Set with glfwSetCursorPosCallback(window, cursor_position_callback);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
//    std::cout << "mouse x,y: " << xpos << ", " << ypos << std::endl;
    
    g_MouseState.currentPositionXY.x = (int)xpos;
    g_MouseState.currentPositionXY.y = (int)ypos;

    if ( g_MouseState.bIsLeftMouseButtonDown )
    {
        // If the Left mouse key is down, 
        //  figure out the differene between the current mouse postion and the last one

        glm::ivec2 deltaMousePosition = g_MouseState.lastPositionXY - g_MouseState.currentPositionXY;

        // Use those values to "turn" the camera
    
        // Move left or right? 
        if ( deltaMousePosition.x != 0 )
        {
            ::g_pFlyCamera->rotateLeftRight_Yaw( (float)deltaMousePosition.x );
        }
        // Mouse move Up or down? 
        if ( deltaMousePosition.y != 0 )
        {
            ::g_pFlyCamera->pitchUpDown((float)deltaMousePosition.y);
        }
    }//if ( g_MouseState.bIsLeftMouseButtonDown )


    // Update the last one
    g_MouseState.lastPositionXY = g_MouseState.currentPositionXY;

    //return;
}

// Set with glfwSetCursorEnterCallback(window, cursor_enter_callback);
void cursor_enter_callback(GLFWwindow* window, int entered)
{
    //if ( entered )
    //{
    //    std::cout << "Mouse is over the window" << std::endl;
    //}
    //else
    //{
    //    std::cout << "Mouse left the window" << std::endl;
    //}
    return;
}


// Set with glfwSetMouseButtonCallback(window, mouse_button_callback);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        switch (action)
        {
        case GLFW_PRESS:
//            std::cout << "GLFW_PRESS" << std::endl;
            g_MouseState.bIsLeftMouseButtonDown = true;
            break;
        case GLFW_RELEASE:
//            std::cout << "GLFW_RELEASE" << std::endl;
            g_MouseState.bIsLeftMouseButtonDown = false;
            break;
        default:
            std::cout << "Mouse button state unknown" << std::endl;
            break;
        }
    }

    return;
}

// Set with glfwSetScrollCallback(window, scroll_callback);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // For most mice, the value is the DIFFERENCE between the last time this was called!
    // (i.e. it's not an absolute value)
    // It also goes up and down along the Y axis and is an integer
//    std::cout << "xoffset, yoffset: " << xoffset << ", " << yoffset << std::endl;

    const float MOUSE_WHEEL_RATIO = 0.1f;

    ::g_pFlyCamera->adjustMovementSpeed( float(yoffset) * MOUSE_WHEEL_RATIO );

    return;
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    const float CAMERA_MOVE_SPEED = 0.1f;

    if (mods == GLFW_MOD_SHIFT)
    {
        if (key == GLFW_KEY_F9 && action == GLFW_PRESS)
        {
            // Save state to file
//            MyAmazingStateThing->saveToFile("MySaveFile.sexy");
        }
//        if (key == GLFW_KEY_F10 && action == GLFW_PRESS)
//        {
//            // Save state to file
//            // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messagebox
////            MessageBox(NULL, L"Hello!", L"The title", MB_OK);
//            if (MessageBox(NULL, L"Kill all humans?", L"Bender asks", MB_YESNO) == IDYES)
//            {
//                std::cout << "You are heartless" << std::endl;
//            }
//            else
//            {
//                std::cout << "Humans still live..." << std::endl;
//            }
//        }
    }//if (mods == GLFW_MOD_SHIFT)

 //   if (mods == GLFW_KEY_LEFT_CONTROL)
    if (isControlDown(window))
    {
        if (key == GLFW_KEY_5 && action == GLFW_PRESS)
        {
            // check if you are out of bounds
            if (::g_selectedLightIndex > 0)
            {

                ::g_selectedLightIndex--;
            }
            //// 0 to 10
            //if (::g_selectedLightIndex < 0)
            //{
            //    ::g_selectedLightIndex = 0;
            //}

        }
        if (key == GLFW_KEY_6 && action == GLFW_PRESS)
        {
            ::g_selectedLightIndex++;
            if (::g_selectedLightIndex >= 10)
            {
                ::g_selectedLightIndex = 9;
            }
        }

        if (key == GLFW_KEY_9 && action == GLFW_PRESS)
        {
            ::g_bShowDebugSpheres = true;
        }
        if (key == GLFW_KEY_0 && action == GLFW_PRESS)
        {
            ::g_bShowDebugSpheres = false;
        }
    }//if (mods == GLFW_KEY_LEFT_CONTROL)

    if (areAllModifiersUp(window))
    {
        {// START OF: change destination path
            bool bDidDestinationChange = false;
            glm::vec3 oldDest = ::g_pTerrainPathChooser->destinationXYZ;
            // Change the destination path location
            if (key == GLFW_KEY_UP )
            {
                oldDest.z += 1.0f;
                bDidDestinationChange = true;
            }
            if (key == GLFW_KEY_DOWN )
            {
                oldDest.z -= 1.0f;
                bDidDestinationChange = true;

            }
            if (key == GLFW_KEY_LEFT )
            {
                oldDest.x -= 1.0f;
                bDidDestinationChange = true;

            }
            if (key == GLFW_KEY_RIGHT )
            {
                oldDest.x += 1.0f;
                bDidDestinationChange = true;
            }
            if (bDidDestinationChange)
            {
                // Adjust it based on the 
                oldDest = ::g_pTerrainPathChooser->getClosestPoint(oldDest);
                ::g_pTerrainPathChooser->destinationXYZ = oldDest;
            }
        }// END OF: change destination path




        if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
        {
            std::ifstream luaCommandFile("lua_commands.txt");
            // Read this file and send each command to lua
            if (luaCommandFile.is_open())
            {
                char lua_commandLine[2048] = { 0 }; // buffer of 2048 chars
                std::stringstream ssLuaCommand;
                while (luaCommandFile.getline(lua_commandLine, 2048))
                {
                    ssLuaCommand << lua_commandLine << '\n';
                }
                ::g_pMyLuaMasterBrain->RunScriptImmediately(ssLuaCommand.str());
            }
        }

        if (key == GLFW_KEY_F10 && action == GLFW_PRESS)
        {
            //// HACK: Prompt on the command line for a lua command to run
            //std::cout << "Enter a Lua command to run:" << std::endl;
            //std::string theLuaCommand;
            //std::cin >> theLuaCommand;
            //std::cout << "Calling lua..." << std::endl;
            //::g_pMyLuaMasterBrain->RunScriptImmediately(theLuaCommand);
            //std::cout << "done." << std::endl;
            // 
            // Read from a command file to tell lua what to do
            std::ifstream luaCommandFile("lua_commands.txt");
            // Read this file and send each command to lua
            if (luaCommandFile.is_open())
            {
                char lua_commandLine[2048] = { 0 }; // buffer of 2048 chars
                while (luaCommandFile.getline(lua_commandLine, 2048))
                {
                    // If the 1st character is a "~" then skip
                    // So we can 'comment out' certain commands
                    if (lua_commandLine[0] != '~')
                    {
                        ::g_pMyLuaMasterBrain->RunScriptImmediately(std::string(lua_commandLine));
                    }
                }
            }
        }

        // Pressing F8 moves the viper to the bunny #15
        if (key == GLFW_KEY_F8 && action == GLFW_PRESS)
        {

            //::g_pMyLuaMasterBrain->RunScriptImmediately(
            //    "AddSerialCommand('Hey hey!')");
            ::g_pMyLuaMasterBrain->RunScriptImmediately(
                "AddSerialCommand('New_Viper_Player', -50.0, 15.0, 30.0, 5.0)");
            ::g_pMyLuaMasterBrain->RunScriptImmediately(
                "AddSerialCommand('New_Viper_Player', 75.0, 10.0, -45.0, 12.5)");

//            ::g_pMyLuaMasterBrain->RunScriptImmediately("a = 9\nb = 7.2\nprint(a + b)\n");

            // Find the bunny and viper
            // Create a move command passing this inforamtion
            // Add it to the command manager thing
 //           cPhysics::sPhysInfo* pViperPhys = ::g_pPhysicEngine->pFindAssociateMeshByFriendlyName("New_Viper_Player");

 //           sMesh* pBunny_15 = pFindMeshByFriendlyName("Bunny_15");
 //           if (pBunny_15 /*&& pViperPhys*/)
 //           {
 //               // both exist
////                cMoveRelativeTime* pMoveViper = new cMoveRelativeTime();
 //               std::vector<std::string> vecCommandDetails;
 //               vecCommandDetails.push_back("New_Viper_Player");    // Object command controls
 //               vecCommandDetails.push_back(::g_floatToString(pBunny_15->positionXYZ.x));
 //               vecCommandDetails.push_back(::g_floatToString(pBunny_15->positionXYZ.y));
 //               vecCommandDetails.push_back(::g_floatToString(pBunny_15->positionXYZ.z));
 //               vecCommandDetails.push_back(::g_floatToString(5.0f));
 //
 //               iCommand* pMoveViper = ::g_pCommandFactory->pCreateCommandObject(
 //                   "Move Relative ConstVelocity+Time", vecCommandDetails);
 //               //moveViper.
////                pMoveViper->Init(pViperPhys, pBunny_15->positionXYZ, 5.0);
 //
 //               // 
 //               ::g_pCommandDirector->addSerial(pMoveViper);
 //           }
 //
 //           sMesh* pBunny_27 = pFindMeshByFriendlyName("Bunny_27");
 //           if (pBunny_27 /*&& pViperPhys*/)
 //           {
 //               // both exist
////                cMoveRelativeTime* pMoveViper = new cMoveRelativeTime();
 //               //moveViper.
////                pMoveViper->Init(pViperPhys, pBunny_27->positionXYZ, 12.5);
 //               std::vector<std::string> vecCommandDetails;
 //               vecCommandDetails.push_back("New_Viper_Player");    // Object command controls
 //               vecCommandDetails.push_back(::g_floatToString(pBunny_27->positionXYZ.x));
 //               vecCommandDetails.push_back(::g_floatToString(pBunny_27->positionXYZ.y));
 //               vecCommandDetails.push_back(::g_floatToString(pBunny_27->positionXYZ.z));
 //               vecCommandDetails.push_back(::g_floatToString(5.0f));
 //
 //               iCommand* pMoveViper = ::g_pCommandFactory->pCreateCommandObject(
 //                   "Move Relative ConstVelocity+Time", vecCommandDetails);
 //
 //               // 
 //               ::g_pCommandDirector->addSerial(pMoveViper);
 //           }

        }//if (key == GLFW_KEY_F8 && action == GLFW_PRESS)

    }//if (areAllModifiersUp(window))



    return;
}