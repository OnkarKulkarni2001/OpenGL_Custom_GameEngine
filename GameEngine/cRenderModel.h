#pragma once
#include "cLoadModels.h"

class cRenderModel
{
public:
	void Render(GLuint shaderProgram, cLoadModels* model);
	void DrawDebugSphere(cLoadModels* sphereModel, glm::vec3 position, glm::vec4 RGBA, float scale, GLuint shaderProgram);
};