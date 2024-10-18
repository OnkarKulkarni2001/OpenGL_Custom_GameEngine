#include "cLightManager.h"

void cLightManager::TurnOnLights(GLuint shaderProgram, int numberOfLights)
{
	lights = new sLights[numberOfLights];
	for (int indexOfLight = 0; indexOfLight != numberOfLights; indexOfLight++) {
		glUniform3fv(glGetUniformLocation(shaderProgram, "light.position"), 1, glm::value_ptr((lights[indexOfLight]).lightPosition));
		glUniform3fv(glGetUniformLocation(shaderProgram, "light.color"), 1, glm::value_ptr((lights[indexOfLight]).lightColor));
	}
}