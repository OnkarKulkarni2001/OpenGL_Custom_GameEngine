#include "cLightManager.h"
#include <fstream>
#include <iostream>

cLightManager::cLightManager()
{
}

cLightManager::~cLightManager()
{
}

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

	while (token != "light_properties") {
		lightsFile >> token;
	}
	
	//pLights = new sLights[numberOfLights];

	for (int indexOfLight = 0; indexOfLight != numberOfLights; indexOfLight++) {
		sLights lights;
		lightsFile >> lights.position.x;
		lightsFile >> lights.position.y;
		lightsFile >> lights.position.z;

		lightsFile >> lights.color.r;
		lightsFile >> lights.color.g;
		lightsFile >> lights.color.b;
		lightsFile >> lights.color.a;

		lightsFile >> lights.ambient.x;
		lightsFile >> lights.ambient.y;
		lightsFile >> lights.ambient.z;
		lightsFile >> lights.ambient.w;

		lightsFile >> lights.diffuse.x;
		lightsFile >> lights.diffuse.y;
		lightsFile >> lights.diffuse.z;
		lightsFile >> lights.diffuse.w;

		lightsFile >> lights.specular.x;
		lightsFile >> lights.specular.y;
		lightsFile >> lights.specular.z;
		lightsFile >> lights.specular.w;

		lightsFile >> lights.atten.x;
		lightsFile >> lights.atten.y;
		lightsFile >> lights.atten.z;
		lightsFile >> lights.atten.w;

		lightsFile >> lights.direction.x;
		lightsFile >> lights.direction.y;
		lightsFile >> lights.direction.z;
		lightsFile >> lights.direction.w;

		lightsFile >> lights.param1.x;
		lightsFile >> lights.param1.y;
		lightsFile >> lights.param1.z;
		lightsFile >> lights.param1.w;

		lightsFile >> lights.param2.x;
		lightsFile >> lights.param2.y;
		lightsFile >> lights.param2.z;
		lightsFile >> lights.param2.w;

		lights.index = indexOfLight;
		this->lights.push_back(lights);	// adding lights to the vector
	}
}

void cLightManager::TurnOnLights(GLuint shaderProgram, int numberOfLights)
{
	//delete[] pLights;
	glUseProgram(shaderProgram);
	//pLights = new sLights[numberOfLights];
	for (int indexOfLight = 0; indexOfLight != numberOfLights; indexOfLight++) {
		std::string lightPosUniform = "pLights[" + std::to_string(indexOfLight) + "].position";
		glUniform3fv(glGetUniformLocation(shaderProgram, lightPosUniform.c_str()), 1, glm::value_ptr((lights[indexOfLight]).position));
		
		std::string lightColUniform = "pLights[" + std::to_string(indexOfLight) + "].color";
		glUniform4fv(glGetUniformLocation(shaderProgram, lightColUniform.c_str()), 1, glm::value_ptr((lights[indexOfLight]).color));
		
		std::string lightAmbiUniform = "pLights[" + std::to_string(indexOfLight) + "].ambient";
		glUniform4fv(glGetUniformLocation(shaderProgram, lightAmbiUniform.c_str()), 1, glm::value_ptr((lights[indexOfLight]).ambient));

		std::string lightDifUniform = "pLights[" + std::to_string(indexOfLight) + "].diffuse";
		glUniform4fv(glGetUniformLocation(shaderProgram, lightDifUniform.c_str()), 1, glm::value_ptr((lights[indexOfLight]).diffuse));
		
		std::string lightSpecUniform = "pLights[" + std::to_string(indexOfLight) + "].specular";
		glUniform4fv(glGetUniformLocation(shaderProgram, lightSpecUniform.c_str()), 1, glm::value_ptr((lights[indexOfLight]).specular));
		
		std::string lightAttenUniform = "pLights[" + std::to_string(indexOfLight) + "].atten";
		glUniform4fv(glGetUniformLocation(shaderProgram, lightAttenUniform.c_str()), 1, glm::value_ptr((lights[indexOfLight]).atten));
		
		std::string lightDirUniform = "pLights[" + std::to_string(indexOfLight) + "].direction";
		glUniform4fv(glGetUniformLocation(shaderProgram, lightDirUniform.c_str()), 1, glm::value_ptr((lights[indexOfLight]).direction));
		
		std::string lightParam1Uniform = "pLights[" + std::to_string(indexOfLight) + "].param1";
		glUniform4fv(glGetUniformLocation(shaderProgram, lightParam1Uniform.c_str()), 1, glm::value_ptr((lights[indexOfLight]).param1));
		
		std::string lightParam2Uniform = "pLights[" + std::to_string(indexOfLight) + "].param2";
		glUniform4fv(glGetUniformLocation(shaderProgram, lightParam2Uniform.c_str()), 1, glm::value_ptr((lights[indexOfLight]).param2));
	}
	glUniform1i(glGetUniformLocation(shaderProgram, "numberOfLights"), numberOfLights);
}