#include "cTerrainPathChooser.h"

cTerrainPathChooser::cTerrainPathChooser(cVAOManager* pTheVAOManager)
{
	this->m_pTheVAOManager = pTheVAOManager;
}

cTerrainPathChooser::~cTerrainPathChooser()
{
}

// Can't call (is private)
cTerrainPathChooser::cTerrainPathChooser()
{
}

glm::vec3 cTerrainPathChooser::getClosestPoint(glm::vec3 testPoint)
{
	return testPoint;
}


// This is called to set the mesh we are "walking" along 
// (i.e. traversing the tree structure inside the PLY model)
// pTerrain->modelFileName = "assets/models/Simple_MeshLab_terrain_x5_xyz_N_uv.ply";
// pTerrain->positionXYZ = glm::vec3(0.0f, -175.0f, 0.0f);
void cTerrainPathChooser::setTerrainMesh(std::string meshName, glm::vec3 meshXYZOffset)
{

	return;
}


// From the start to the end, calculate a list of vertices
//	along the path from start to finish.
// The idea is that we'd draw a debug sphere a each of these
//	locations in the vector.
// This is called every frame
void cTerrainPathChooser::CalculatePath(std::vector<glm::vec3>& vecVertexPathLocations)
{
	glm::vec3 curLoc = this->startXYZ;
	while (curLoc != this->destinationXYZ)		// **DANGER! WILL ROBINSON!**
	{
		// Pick the next spot
		// Add current to the vecVertexPathLocations
	}


	return;
}
