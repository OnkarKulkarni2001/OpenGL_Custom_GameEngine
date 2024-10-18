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

class cLightManager
{
	struct sLights {
		glm::vec3 lightPosition;
		glm::vec3 lightColor;
	};

	sLights* lights;

	void TurnOnLights(GLuint shaderProgram, int numberOfLights);
};