#pragma once
#include "cLightManager.h"
#include "FlyCam.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

class cLightMover {
public:
	cLightManager::sLights* pLight;
	FlyCam* flyCam = nullptr;

	float lightSpeed = 0.01f;
	unsigned int numberOfLights;

	glm::vec3 lightPosition;
	glm::vec3 lightForwardVector;
	glm::vec3 lightRightVector;
	glm::vec3 lightUpVector;

	cLightMover(cLightManager& light, FlyCam& flyCam, unsigned int numberOfLights);
	~cLightMover();

	void MoveForward(cLightManager::sLights& light);
	void MoveBackward(cLightManager::sLights& light);
	void MoveLeft(cLightManager::sLights& light);
	void MoveRight(cLightManager::sLights& light);
	void MoveUp(cLightManager::sLights& light);
	void MoveDown(cLightManager::sLights& light);
};