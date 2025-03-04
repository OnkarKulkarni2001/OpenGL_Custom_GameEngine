#pragma once

// This is the 3D drawing information
//	to draw a single mesh (single PLY file)

#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include <string>

struct sMesh
{
	sMesh();
	std::string modelFileName;			// "bunny.ply"
	std::string uniqueFriendlyName;		// "Fred", "Ali", "Bunny_007"

	glm::vec3 positionXYZ;
	glm::vec3 rotationEulerXYZ;		// 90 degrees around the x axis
	float uniformScale = 1.0f;				// Same for each axis

	glm::mat4 calcMatModel(void);

	glm::vec4 objectColourRGBA;		// 0 - 1.0 
	// If true, it uses the colour above
	bool bOverrideObjectColour = true;

	bool bIsWireframe = false;
	bool bIsVisible = true;
	bool bDoNotLight = false;

	// unique ID is read only
	unsigned int getUniqueID(void);
private:
	unsigned int m_uniqueID = 0;
	// 
	static unsigned int m_NextUniqueID;// = 100;
public:

//	std::string textureName;
	static const unsigned int MAX_NUM_TEXTURES = 8;
	std::string textures[MAX_NUM_TEXTURES];
	float blendRatio[MAX_NUM_TEXTURES];

	// 0.0 = invisible
	// 1.0 = solid 
	float alphaTransparency = 1.0f;

	std::string getState(void);
	bool loadState(std::string newState);

//	sMesh* pChildMeshes[100];
	std::vector< sMesh* > vec_pChildMeshes;


	// I'm going to remove this...
	// //
	// Relative (start where I'm at, then move to endXYZ)
//	void MoveTo(glm::vec3 endXYZ, double timeToMove);
	//
//	void MoveTo(glm::vec3 startXYZ, glm::vec3 endXYZ, double timeToMove);
	//
//	void MoveTo(glm::vec3 startXYZ, glm::vec3 endXYZ, double maxVelocity);

};