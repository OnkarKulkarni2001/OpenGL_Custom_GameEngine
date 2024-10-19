#include "cLightMover.h"


cLightMover::cLightMover(cLightManager::sLights& light, FlyCam& flyCam)
{
	this->pLight = &light;
	lightPosition = (&light)->position;
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
	pLight->position = glm::vec4(lightPosition, 1.0f);
	//std::string lightPosUniform = "pLights[" + std::to_string(pLight->index) + "].position";
	//glUniform3fv(glGetUniformLocation(shaderProgram, lightPosUniform.c_str()), 1, glm::value_ptr((lights[indexOfLight]).position));
	//light->position.x = lightPosition.x;
	//light->position.y = lightPosition.y;
	//light->position.z = lightPosition.z;
}

void cLightMover::MoveBackward()
{
	lightPosition -= lightSpeed * lightForwardVector;
	pLight->position = glm::vec4(lightPosition,1.0f);
	//light->position.x = lightPosition.x;
	//light->position.y = lightPosition.y;
	//light->position.z = lightPosition.z;
}

void cLightMover::MoveLeft()
{
	lightPosition -= lightSpeed * lightRightVector;
	//pLight->position = glm::vec4(lightPosition, 1.0f);
	pLight->position.x = lightPosition.x;
	pLight->position.y = lightPosition.y;
	pLight->position.z = lightPosition.z;
	std::cout << lightPosition.x << " " << lightPosition.y << " " << lightPosition.z << "lightMover" << std::endl;
	std::cout << pLight->position.x << " " << pLight->position.y << " " << pLight->position.z << "originalLight " << std::endl;
}

void cLightMover::MoveRight()
{
	lightPosition += lightSpeed * lightRightVector;
	pLight->position = glm::vec4(lightPosition, 1.0f);
	//light->position.x = lightPosition.x;
	//light->position.y = lightPosition.y;
	//light->position.z = lightPosition.z;
}

void cLightMover::MoveUp()
{
	lightPosition += lightSpeed * lightUpVector;
	pLight->position = glm::vec4(lightPosition, 1.0f);
	//light->position.x = lightPosition.x;
	//light->position.y = lightPosition.y;
	//light->position.z = lightPosition.z;
}

void cLightMover::MoveDown()
{
	lightPosition -= lightSpeed * lightUpVector;
	pLight->position = glm::vec4(lightPosition, 1.0f);
	//light->position.x = lightPosition.x;
	//light->position.y = lightPosition.y;
	//light->position.z = lightPosition.z;
}
