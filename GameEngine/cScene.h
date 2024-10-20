#pragma once
#include "cLoadModels.h"
#include <string>
#include <vector>
#include <map>

class cScene {
public:
	unsigned int numberOfMeshesToLoad = 0;
	std::string meshPath = "";

	//struct sMeshTransform {
	//	float x, y, z, xScale, yScale, zScale, xRotation, yRotation, zRotation;
	//};
	//
	//sMeshTransform* pMeshTransform;
	std::vector<std::string> pathOfMeshesToLoad;
	std::vector<cLoadModels> pModels;

	//void ExportMaterial(GLuint shaderProgram, int numberOfMaterials);		// TODO:
	void CreateScene(std::string sceneFileName);
};