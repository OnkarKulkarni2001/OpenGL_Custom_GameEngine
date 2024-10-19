#include "cLightManager.h"
#include <fstream>
#include <iostream>

void cLightManager::LoadLights(std::string lightsFilePath)
{
	//std::string lightsFilePath = "D:/Graphics1/GameEngine/lightsFile.txt";
	std::string token = "";

	std::ifstream lightsFile(lightsFilePath);
	if (!lightsFile.is_open()) {
		std::cout << "Unable to open lights file!" << std::endl;
		return;
	}
	else {
		std::cout << "Lights file opened successfully!" << std::endl;
	}

	while (token != "number_of_lights") {
		lightsFile >> token;
	}
	lightsFile >> numberOfLights;

	while (token != "light_positions_colors") {
		lightsFile >> token;
	}
	
	lights = new sLights[numberOfLights];

	for (int indexOfLight = 0; indexOfLight != numberOfLights; indexOfLight++) {
		lightsFile >> lights[indexOfLight].lightPosition.x;
		lightsFile >> lights[indexOfLight].lightPosition.y;
		lightsFile >> lights[indexOfLight].lightPosition.z;

		lightsFile >> lights[indexOfLight].lightColor.r;
		lightsFile >> lights[indexOfLight].lightColor.g;
		lightsFile >> lights[indexOfLight].lightColor.b;
		lightsFile >> lights[indexOfLight].lightColor.a;
	}
}

void cLightManager::TurnOnLights(GLuint shaderProgram, int numberOfLights)
{
	delete[] lights;

	lights = new sLights[numberOfLights];
	for (int indexOfLight = 0; indexOfLight != numberOfLights; indexOfLight++) {
		std::string lightPosUniform = "lights[" + std::to_string(indexOfLight) + "].lightPosition";
		glUniform3fv(glGetUniformLocation(shaderProgram, lightPosUniform.c_str()), 1, glm::value_ptr((lights[indexOfLight]).lightPosition));
		
		std::string lightColUniform = "lights[" + std::to_string(indexOfLight) + "].lightColor";
		glUniform4fv(glGetUniformLocation(shaderProgram, lightColUniform.c_str()), 1, glm::value_ptr((lights[indexOfLight]).lightColor));
	}
	glUniform1i(glGetUniformLocation(shaderProgram, "numberOfLights"), numberOfLights);
}