#include "cRenderModel.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void cRenderModel::Render(GLuint shaderProgram, cLoadModels* model)
{
	if (model->bIsWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glBindVertexArray(model->VAO_ID);

	glm::mat4 modelMat = model->CreateModelMatrix(shaderProgram, *model);      // Creation of model matrix with arguements passed in sceneFile.txt
	model->GenerateTransformedVertices(modelMat);            // this is here because collisions want transformed vertices
	
	unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));

	glDrawElements(GL_TRIANGLES, model->numberOfIndices, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}