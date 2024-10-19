#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <vector>
#include <string>

struct sLights {
	glm::vec4 position;		// position for point lights
	glm::vec4 color;
	glm::vec4 ambient;
	glm::vec4 diffuse;		// diffuse color
	glm::vec4 specular;		// specular color (w = specular power)
	glm::vec4 atten;		// (constant, linear, quadratic, cutoff)
	glm::vec4 direction;	// for directional lights/ spot lights
	glm::vec4 param1;		// x = light type, y = inner angle, z = outer angle
	glm::vec4 param2;		// x = on/off
};

class cLightManager
{
public:
	int numberOfLights;
	//sLights* pLights;

	std::vector<sLights> lights;

	cLightManager();
	~cLightManager();

	void LoadLights(std::string lightsFilePath);
	void TurnOnLights(GLuint shaderProgram, int numberOfLights);
};