#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include "cVAOManager/cVAOManager.h"


class cTerrainPathChooser
{
public:
	cTerrainPathChooser(cVAOManager* pTheVAOManager);
	~cTerrainPathChooser();

	glm::vec3 startXYZ = glm::vec3(0.0f);
	glm::vec3 destinationXYZ = glm::vec3(0.0f);	// We can move this one

	// This will return the closest vertex on the terrain to this point.
	// (We can use this to pick a starting and ending point)
	glm::vec3 getClosestPoint(glm::vec3 testPoint);

	// This is called to set the mesh we are "walking" along 
	// (i.e. traversing the tree structure inside the PLY model)
	// pTerrain->modelFileName = "assets/models/Simple_MeshLab_terrain_x5_xyz_N_uv.ply";
	// pTerrain->positionXYZ = glm::vec3(0.0f, -175.0f, 0.0f);
	void setTerrainMesh(std::string meshName, glm::vec3 meshXYZOffset);


	// From the start to the end, calculate a list of vertices
	//	along the path from start to finish.
	// The idea is that we'd draw a debug sphere a each of these
	//	locations in the vector.
	// This is called every frame
	void CalculatePath(std::vector<glm::vec3>& vecVertexPathLocations);

private:
	cTerrainPathChooser();
	cVAOManager* m_pTheVAOManager = NULL;
	glm::vec3 m_meshXYZOffset = glm::vec3(0.0f);
	std::string m_meshName;
};