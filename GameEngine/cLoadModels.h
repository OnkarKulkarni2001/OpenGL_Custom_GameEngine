#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

using namespace std;
class cLoadModels {
public:
	string ModelFileName;
	int numberOfVertices;
	int numberOfFaces;
	unsigned int numberOfVerticesToRender;
	int discard;

	struct sVertex {
		float x;
		float y;
		float z;
	};

	struct sFaces {
		unsigned int vertexNumber1;
		unsigned int vertexNumber2;
		unsigned int vertexNumber3;
	};

	struct sVerticesToRender {
		glm::vec3 vertexPosition;
		glm::vec4 vertexColor;
	};

	struct sMeshTransform {
		float x, y, z, xScale, yScale, zScale, xRotation, yRotation, zRotation;
	};

	sMeshTransform pMeshTransform;
	sVertex* pVertex;
	sFaces* pFaces;
	sVerticesToRender* pVerticesToRender;

	void LoadPlyModel(string ModelFileName);
	glm::mat4 CreateModelMatrix(GLuint shaderProgram, cLoadModels plyModel);
};