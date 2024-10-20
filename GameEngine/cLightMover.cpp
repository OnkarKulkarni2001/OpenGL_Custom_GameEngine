#include "cLightMover.h"
#include <iostream>

cLightMover::cLightMover(cLightManager& light, FlyCam& flyCam, unsigned int numberOfLights) : flyCam(&flyCam), numberOfLights(numberOfLights)
{
	lightForwardVector = flyCam.camForwardVector;
	lightRightVector = flyCam.camRightVector;
	lightUpVector = flyCam.camUpVector;
}

cLightMover::~cLightMover()
{

}

void cLightMover::MoveForward(cLightManager::sLights& light)
{
	lightPosition += lightSpeed * lightForwardVector;
	light.position = lightPosition;
	std::cout << lightPosition.x << " " << lightPosition.y << " " << lightPosition.z << " lightMover" << std::endl;
}

void cLightMover::MoveBackward(cLightManager::sLights& light)
{
	lightPosition -= lightSpeed * lightForwardVector;
	light.position = lightPosition;
	std::cout << lightPosition.x << " " << lightPosition.y << " " << lightPosition.z << " lightMover" << std::endl;
}

void cLightMover::MoveLeft(cLightManager::sLights& light)
{
	lightPosition -= lightSpeed * lightRightVector;
	light.position = lightPosition;
	std::cout << lightPosition.x << " " << lightPosition.y << " " << lightPosition.z << " lightMover" << std::endl;
}

void cLightMover::MoveRight(cLightManager::sLights& light)
{
	lightPosition += lightSpeed * lightRightVector;
	light.position = lightPosition;
	std::cout << lightPosition.x << " " << lightPosition.y << " " << lightPosition.z << " lightMover" << std::endl;
}

void cLightMover::MoveUp(cLightManager::sLights& light)
{
	lightPosition += lightSpeed * lightUpVector;
	light.position = lightPosition;
	std::cout << lightPosition.x << " " << lightPosition.y << " " << lightPosition.z << " lightMover" << std::endl;
}

void cLightMover::MoveDown(cLightManager::sLights& light)
{
	lightPosition -= lightSpeed * lightUpVector;
	light.position = lightPosition;
	std::cout << lightPosition.x << " " << lightPosition.y << " " << lightPosition.z << " lightMover" << std::endl;
}