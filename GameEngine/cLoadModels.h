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
		float x, y, z;
		float nx, ny, nz;
		float r, g, b, a; // Colors (RGBA)
	};

	struct sFaces {
		unsigned int vertexNumber1;
		unsigned int vertexNumber2;
		unsigned int vertexNumber3;
	};

	struct sVerticesToRender {
		glm::vec3 vertexPosition;
		glm::vec3 vertexNormal;
		glm::vec4 vertexColor;
	};

	struct sMeshTransform {
		float x, y, z, xScale, yScale, zScale, xRotation, yRotation, zRotation;
	};

	//struct sMaterial {
	//	float shininess;
	//	glm::vec3 diffuse;
	//	glm::vec3 specular;
	//};

	//cPhysics::sPhysicsMesh physicsMesh;
	sMeshTransform pMeshTransform;
	sVertex* pVertex;
	sFaces* pFaces;
	sVerticesToRender* pVerticesToRender;
/*	sMaterial pMaterial;

	sMaterial* vMaterial;*/		// this dynamic array is added just because I wanted to access index of materials from ExportMaterial function.

	void LoadPlyModel(string ModelFileName);
	glm::mat4 CreateModelMatrix(GLuint shaderProgram, cLoadModels plyModel);
};