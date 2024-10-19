#include "cLightMover.h"


cLightMover::cLightMover(sLights* light, FlyCam& flyCam)
{
	this->light = light;
	lightPosition = glm::vec3(light->position);
	lightForwardVector = flyCam.camForwardVector;
	lightRightVector = flyCam.camRightVector;
	lightUpVector = flyCam.camUpVector;
}

cLightMover::~cLightMover()
{
}

void cLightMover::MoveForward()
{
	lightPosition += lightSpeed * lightForwardVector;
	light->position = glm::vec4(lightPosition, 1.0f);
	//light->position.x = lightPosition.x;
	//light->position.y = lightPosition.y;
	//light->position.z = lightPosition.z;
}

void cLightMover::MoveBackward()
{
	lightPosition -= lightSpeed * lightForwardVector;
	light->position = glm::vec4(lightPosition,1.0f);
	//light->position.x = lightPosition.x;
	//light->position.y = lightPosition.y;
	//light->position.z = lightPosition.z;
}

void cLightMover::MoveLeft()
{
	lightPosition -= lightSpeed * lightRightVector;
	light->position = glm::vec4(lightPosition, 1.0f);
	//light->position.x = lightPosition.x;
	//light->position.y = lightPosition.y;
	//light->position.z = lightPosition.z;
}

void cLightMover::MoveRight()
{
	lightPosition += lightSpeed * lightRightVector;
	light->position = glm::vec4(lightPosition, 1.0f);
	//light->position.x = lightPosition.x;
	//light->position.y = lightPosition.y;
	//light->position.z = lightPosition.z;
}

void cLightMover::MoveUp()
{
	lightPosition += lightSpeed * lightUpVector;
	light->position = glm::vec4(lightPosition, 1.0f);
	//light->position.x = lightPosition.x;
	//light->position.y = lightPosition.y;
	//light->position.z = lightPosition.z;
}

void cLightMover::MoveDown()
{
	lightPosition -= lightSpeed * lightUpVector;
	light->position = glm::vec4(lightPosition, 1.0f);
	//light->position.x = lightPosition.x;
	//light->position.y = lightPosition.y;
	//light->position.z = lightPosition.z;
}
