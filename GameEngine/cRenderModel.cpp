#include "cRenderModel.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void cRenderModel::Render(GLuint shaderProgram, cLoadModels* model)
{
	glBindVertexArray(model->VAO_ID);

	glm::mat4 modelMat = model->CreateModelMatrix(shaderProgram, *model);      // Creation of model matrix with arguements passed in sceneFile.txt
	model->GenerateTransformedVertices(modelMat);            // this is here because collisions want transformed vertices
	
	unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));

	glDrawElements(GL_TRIANGLES, model->numberOfIndices, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}

void cRenderModel::DrawDebugSphere(cLoadModels* sphereModel, glm::vec3 position, glm::vec4 RGBA, float scale, GLuint shaderProgram)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	sphereModel->pMeshTransform.x = position.x;
	sphereModel->pMeshTransform.y = position.y;
	sphereModel->pMeshTransform.z = position.z;
	
	sphereModel->pVertex->r = RGBA.r;
	sphereModel->pVertex->g = RGBA.g;
	sphereModel->pVertex->b = RGBA.b;
	sphereModel->pVertex->a = RGBA.a;

	sphereModel->scale.x = scale;
	sphereModel->scale.y = scale;
	sphereModel->scale.z = scale;

	sphereModel->bIsWireframe = true;

    Render(shaderProgram, sphereModel);
}