#include "cLoadModels.h"

cLoadModels::cLoadModels()
{
	this->bIsVisible = true;
	this->bIsWireframe = false;
}

cLoadModels::~cLoadModels()
{
}

void cLoadModels::LoadPlyModel(string ModelFileName) {
	ifstream plyModelFile(ModelFileName);

	string token = "";

	if (!plyModelFile.is_open()) {
		cout << "Trying to open file: " << ModelFileName << endl;
		cout << "Unable to open file because it is already open!" << endl;
		return;
	}
	else {
		cout << "Plyfile open successful!" << endl;
	}

	while (token != "vertex") {
		plyModelFile >> token;
	}
	plyModelFile >> numberOfVertices;	// Just storing numberOfVertices from plyfile!

	while (token != "face") {
		plyModelFile >> token;
	}
	plyModelFile >> numberOfFaces;		// Just storing numberOfFaces from plyfile!

	while (token != "end_header") {
		plyModelFile >> token;
	}

	//numberOfVerticesToRender = numberOfFaces * 3;
	numberOfIndices = numberOfFaces * 3;		// New for implementing separate VAO, VBO, IBO, earlier it was numberOfVertices = numberOfFaces * 3;
	
	pVertex = new sVertex[numberOfVertices];
	pFaces = new sFaces[numberOfFaces];
	//pVerticesToRender = new sVerticesToRender[numberOfVerticesToRender];
	pIndices = new unsigned int[numberOfIndices];

	for (unsigned int vertexIterator = 0; vertexIterator != numberOfVertices; vertexIterator++) {
		plyModelFile >> pVertex[vertexIterator].x;
		plyModelFile >> pVertex[vertexIterator].y;
		plyModelFile >> pVertex[vertexIterator].z;
		plyModelFile >> pVertex[vertexIterator].nx;
		plyModelFile >> pVertex[vertexIterator].ny;
		plyModelFile >> pVertex[vertexIterator].nz;
		plyModelFile >> pVertex[vertexIterator].r;
		plyModelFile >> pVertex[vertexIterator].g;
		plyModelFile >> pVertex[vertexIterator].b;
		plyModelFile >> pVertex[vertexIterator].a;

	}	// storing the vertex co-ordinates

	for (unsigned int faceIterator = 0; faceIterator != numberOfFaces; faceIterator++) {
		discard = 0;
		plyModelFile >> discard;
		plyModelFile >> pFaces[faceIterator].vertexNumber1;
		plyModelFile >> pFaces[faceIterator].vertexNumber2;
		plyModelFile >> pFaces[faceIterator].vertexNumber3;
	}	// storing the vertices contributing to a face


	//unsigned int vertexIndex = 0;
	//for (unsigned int indexOfFace = 0; indexOfFace != numberOfFaces; indexOfFace++) {
	//	pVerticesToRender[vertexIndex + 0].vertexPosition.x = pVertex[pFaces[indexOfFace].vertexNumber1].x;		// Vertex1 of face
	//	pVerticesToRender[vertexIndex + 0].vertexPosition.y = pVertex[pFaces[indexOfFace].vertexNumber1].y;
	//	pVerticesToRender[vertexIndex + 0].vertexPosition.z = pVertex[pFaces[indexOfFace].vertexNumber1].z;		
	//	pVerticesToRender[vertexIndex + 0].vertexNormal.x = pVertex[pFaces[indexOfFace].vertexNumber1].nx;
	//	pVerticesToRender[vertexIndex + 0].vertexNormal.y = pVertex[pFaces[indexOfFace].vertexNumber1].ny;
	//	pVerticesToRender[vertexIndex + 0].vertexNormal.z = pVertex[pFaces[indexOfFace].vertexNumber1].nz;
	//	pVerticesToRender[vertexIndex + 0].vertexColor.r = pVertex[pFaces[indexOfFace].vertexNumber1].r;
	//	pVerticesToRender[vertexIndex + 0].vertexColor.g = pVertex[pFaces[indexOfFace].vertexNumber1].g;
	//	pVerticesToRender[vertexIndex + 0].vertexColor.b = pVertex[pFaces[indexOfFace].vertexNumber1].b;
	//	pVerticesToRender[vertexIndex + 0].vertexColor.a = pVertex[pFaces[indexOfFace].vertexNumber1].a;
	//
	//	pVerticesToRender[vertexIndex + 1].vertexPosition.x = pVertex[pFaces[indexOfFace].vertexNumber2].x;		// Vertex2 of face
	//	pVerticesToRender[vertexIndex + 1].vertexPosition.y = pVertex[pFaces[indexOfFace].vertexNumber2].y;
	//	pVerticesToRender[vertexIndex + 1].vertexPosition.z = pVertex[pFaces[indexOfFace].vertexNumber2].z;
	//	pVerticesToRender[vertexIndex + 1].vertexNormal.x = pVertex[pFaces[indexOfFace].vertexNumber2].nx;
	//	pVerticesToRender[vertexIndex + 1].vertexNormal.y = pVertex[pFaces[indexOfFace].vertexNumber2].ny;
	//	pVerticesToRender[vertexIndex + 1].vertexNormal.z = pVertex[pFaces[indexOfFace].vertexNumber2].nz;
	//	pVerticesToRender[vertexIndex + 1].vertexColor.r = pVertex[pFaces[indexOfFace].vertexNumber1].r;
	//	pVerticesToRender[vertexIndex + 1].vertexColor.g = pVertex[pFaces[indexOfFace].vertexNumber1].g;
	//	pVerticesToRender[vertexIndex + 1].vertexColor.b = pVertex[pFaces[indexOfFace].vertexNumber1].b;
	//	pVerticesToRender[vertexIndex + 1].vertexColor.a = pVertex[pFaces[indexOfFace].vertexNumber1].a;
	//	
	//	pVerticesToRender[vertexIndex + 2].vertexPosition.x = pVertex[pFaces[indexOfFace].vertexNumber3].x;		// Vertex3 of face
	//	pVerticesToRender[vertexIndex + 2].vertexPosition.y = pVertex[pFaces[indexOfFace].vertexNumber3].y;
	//	pVerticesToRender[vertexIndex + 2].vertexPosition.z = pVertex[pFaces[indexOfFace].vertexNumber3].z;
	//	pVerticesToRender[vertexIndex + 2].vertexNormal.x = pVertex[pFaces[indexOfFace].vertexNumber3].nx;
	//	pVerticesToRender[vertexIndex + 2].vertexNormal.y = pVertex[pFaces[indexOfFace].vertexNumber3].ny;
	//	pVerticesToRender[vertexIndex + 2].vertexNormal.z = pVertex[pFaces[indexOfFace].vertexNumber3].nz;
	//	pVerticesToRender[vertexIndex + 2].vertexColor.r = pVertex[pFaces[indexOfFace].vertexNumber1].r;
	//	pVerticesToRender[vertexIndex + 2].vertexColor.g = pVertex[pFaces[indexOfFace].vertexNumber1].g;
	//	pVerticesToRender[vertexIndex + 2].vertexColor.b = pVertex[pFaces[indexOfFace].vertexNumber1].b;
	//	pVerticesToRender[vertexIndex + 2].vertexColor.a = pVertex[pFaces[indexOfFace].vertexNumber1].a;
	//
	//	vertexIndex += 3;
	//}	// copying over the vertex positions from triangle's(face's) vertices to pVerticesToRender and also setting the color of pVerticesToRender to white.	 // 

	// This is new for IBO
	unsigned int index = 0;
	for (unsigned int triIndex = 0; triIndex != numberOfFaces; triIndex++)
	{
		pIndices[index + 0] = pFaces[triIndex].vertexNumber1;
		pIndices[index + 1] = pFaces[triIndex].vertexNumber2;
		pIndices[index + 2] = pFaces[triIndex].vertexNumber3;
		index += 3;
	}

}

void cLoadModels::GenerateTransformedVertices(glm::mat4 model)
{
	for (int vertexIndex = 0; vertexIndex != numberOfVertices; vertexIndex++) {
		glm::vec4 originalVertex = glm::vec4(pVertex[vertexIndex].x, pVertex[vertexIndex].y, pVertex[vertexIndex].z, 1.0f);
		glm::vec4 tempTransformedVertex = model * originalVertex;
		pTransformedVertices[vertexIndex].transformedVertex = glm::vec3(tempTransformedVertex);
	}
}

glm::mat4 cLoadModels::CreateModelMatrix(GLuint shaderProgram, cLoadModels plyModel) {
	glm::mat4 model = glm::mat4(1.0f);
	model = translate(model, glm::vec3(plyModel.pMeshTransform.x , plyModel.pMeshTransform.y, plyModel.pMeshTransform.z));
	
	// Step 2: Apply rotations (Z, Y, X order)
	// Rotations are applied in the reverse order (Z -> Y -> X)
	model = glm::rotate(model, glm::radians(plyModel.pMeshTransform.zRotation), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z-axis
	model = glm::rotate(model, glm::radians(plyModel.pMeshTransform.yRotation), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis
	model = glm::rotate(model, glm::radians(plyModel.pMeshTransform.xRotation), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X-axis

	// Step 3: Apply scaling
	model = glm::scale(model, glm::vec3(
		plyModel.pMeshTransform.xScale,
		plyModel.pMeshTransform.yScale,
		plyModel.pMeshTransform.zScale
	));

	GLuint modelLocation = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	return model;
}
