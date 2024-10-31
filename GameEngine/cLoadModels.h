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
	unsigned int numberOfVertices;
	unsigned int numberOfFaces;
	//unsigned int numberOfVerticesToRender;
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

	//struct sVerticesToRender {
	//	glm::vec3 vertexPosition;
	//	glm::vec3 vertexNormal;
	//	glm::vec4 vertexColor;
	//};

	struct sMeshTransform {
		float x, y, z, xScale, yScale, zScale, xRotation, yRotation, zRotation;
	};

	struct sTransformedVertex {
		glm::vec3 transformedVertex;
	};

	//struct sModelDrawingDetails {
		//std::string modelName;
		
		unsigned int VAO_ID;
		unsigned int VBO_ID;		// this one is normal buffer
		unsigned int IBO_ID;		// IBO(Index Buffer Object): this one is specifically for loading single model but drawing it multiple times

		unsigned int VBO_Offset;	// this is starting index of VBO
		unsigned int IBO_Offset;	// this is starting index of IBO

		unsigned int numberOfIndices;	// this is what I had earlier which was named as numberOfVerticesToRender on line 21, which is numberOfFaces * 3;

		bool bIsWireframe = false;
		bool bIsVisible = true;
	//};

	glm::vec3 position, scale, rotation;

	//struct sMaterial {
	//	float shininess;
	//	glm::vec3 diffuse;
	//	glm::vec3 specular;
	//};

	//cPhysics::sPhysicsMesh physicsMesh;
	sMeshTransform pMeshTransform;			// This one is placed with a line space below because this is different and not a pointer

	sTransformedVertex* pTransformedVertices;
	sVertex* pVertex;
	sFaces* pFaces;
	//sVerticesToRender* pVerticesToRender;
	unsigned int* pIndices;					// This is new for IBO earlier I was using sVerticesToRender* pVerticesToRender;
	
/*	sMaterial pMaterial;

	sMaterial* vMaterial;*/		// this dynamic array is added just because I wanted to access index of materials from ExportMaterial function.

	cLoadModels();
	~cLoadModels();

	void LoadPlyModel(string ModelFileName);
	void GenerateTransformedVertices(glm::mat4 model);
	glm::mat4 CreateModelMatrix(GLuint shaderProgram, cLoadModels plyModel);
};