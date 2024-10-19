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
	FlyCam* flyCam = nullptr;
	cLightManager::sLights* pLight = nullptr;

	glm::vec3 lightPosition;
	glm::vec3 lightForwardVector;
	glm::vec3 lightRightVector;
	glm::vec3 lightUpVector;

	cLightMover(cLightManager::sLights& light, FlyCam& flyCam);
	~cLightMover();

	float lightSpeed = 100.0f;
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();
};