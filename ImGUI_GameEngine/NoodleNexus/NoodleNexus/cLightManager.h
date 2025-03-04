#pragma once

#include "GLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include <string>  

class cLightManager
{
public:
    // This is inside our shader
    //struct sLight
    //{
    //    vec4 position;
    //    vec4 diffuse;
    //    vec4 specular;	// rgb = highlight colour, w = power
    //    vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
    //    vec4 direction;	// Spot, directional lights
    //    vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
    //    // 0 = pointlight
    //    // 1 = spot light
    //    // 2 = directional light
    //    vec4 param2;	// x = 0 for off, 1 for on
    //    // yzw are TBD
    //};
    //const int NUMBEROFLIGHTS = 10;
    //uniform sLight theLights[NUMBEROFLIGHTS];

    cLightManager();

    void loadUniformLocations(GLuint shaderProgram);
    void renderLightDebug();
    void updateShaderWithLightInfo(void);

    // Mirror of what's inside the shader
    struct sLight
    {
        glm::vec4 position;
        glm::vec4 diffuse;
        glm::vec4 specular;	// rgb = highlight colour, w = power
        glm::vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
        glm::vec4 direction;	// Spot, directional lights
        glm::vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
        // 0 = pointlight
        // 1 = spot light
        // 2 = directional light
        glm::vec4 param2;	// x = 0 for off, 1 for on
        // yzw are TBD

        void setLinearAtten(float newLinear)
        {
            this->atten.y = newLinear;
        }
        void setQuadraticAtten(float newQuad)
        {
            this->atten.z = newQuad;
        }
        void TurnOn(void)
        {
            this->param2.x = 1.0f;
        }
        void TurnOff(void)
        {
            this->param2.x = 0.0f;
        }

        // The uniform locations of >this< light
        GLuint position_UL = -1;
        GLuint diffuse_UL = -1;
        GLuint specular_UL = -1;
        GLuint atten_UL = -1;
        GLuint direction_UL = -1;
        GLuint param1_UL = -1;
        GLuint param2_UL = -1;

        std::string getState(void);
        bool loadState(std::string stateString);
    };

    static const int NUMBEROFLIGHTS = 10;
    sLight theLights[NUMBEROFLIGHTS];


    //std::vector<sLight> vecLights;
};

