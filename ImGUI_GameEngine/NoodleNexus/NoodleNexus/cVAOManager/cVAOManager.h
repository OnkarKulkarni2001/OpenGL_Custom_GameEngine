#ifndef _cVAOManager_HG_
#define _cVAOManager_HG_

// Will load the models and place them 
// into the vertex and index buffers to be drawn

#include <string>
#include <map>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>

// The vertex structure 
//	that's ON THE GPU (eventually) 
// So dictated from THE SHADER
//struct sVertex_SHADER_FORMAT_xyz_rgb
//{
//	float x, y, z;		// in vec3 vPos;
//	float r, g, b;		// in vec3 vCol;
//};

//struct sVertex_SHADER_FORMAT_xyz_rgb_N
//{
//	float x, y, z;		// in vec3 vPos;
//	float r, g, b;		// in vec3 vCol;
//	float nx, ny, nz;	// in vec3 vNormal;
//};

struct sVertex_SHADER_FORMAT_xyz_rgb_N_UV
{
	float x, y, z;		// in vec3 vPos;
	float r, g, b;		// in vec3 vCol;
	float nx, ny, nz;	// in vec3 vNormal;
	float u, v;			// in vec2 vUV;
};

struct sModelDrawInfo
{
	sModelDrawInfo(); 

	std::string meshName;

	unsigned int VAO_ID;

	unsigned int VertexBufferID;
	unsigned int VertexBuffer_Start_Index;
	unsigned int numberOfVertices;

	unsigned int IndexBufferID;
	unsigned int IndexBuffer_Start_Index;
	unsigned int numberOfIndices;
	unsigned int numberOfTriangles;

	// The "local" (i.e. "CPU side" temporary array)
//	sVertex_SHADER_FORMAT_xyz_rgb* pVertices;	//  = 0;
//	sVertex_SHADER_FORMAT_xyz_rgb_N* pVertices;	//  = 0;
	sVertex_SHADER_FORMAT_xyz_rgb_N_UV* pVertices;	//  = 0;
	// The index buffer (CPU side)
	unsigned int* pIndices;
	// 
	glm::vec3 maxXYZ, minXYZ, extenXYZ, centreXYZ;
	void calculateExtents(void);

	// For async loading
	bool bIsLoaded = false;			// Thread has loaded file
	bool bIsCopiedToVAO = false;	// File is loaded, ready to go to the VAO
};


class cVAOManager
{
public:

	cVAOManager();
	~cVAOManager();

	bool LoadModelIntoVAO(std::string fileName, 
						  sModelDrawInfo &drawInfo, 
						  unsigned int shaderProgramID);

	// This will load models on a separate thread
	// (i.e. it will return right away)
	bool LoadModelIntoVAO_Async(std::string fileName, 
						  sModelDrawInfo &drawInfo, 
						  unsigned int shaderProgramID);


	// This is called every frame to deal with 
	//	any loaded models from the async thread
	void LoadAsynModels(unsigned int shaderProgramID);

	// Reads the PLY file, loading it into an sModelDrawInfo structure.
	bool m_readPlyFile_XYZ(sModelDrawInfo& modelDrawInfo);
	bool m_readPlyFile_XYZ_Normal(sModelDrawInfo& modelDrawInfo);
	bool m_readPlyFile_XYZ_Normal_UV(sModelDrawInfo& modelDrawInfo);

private:

	// Called by c'tor
	void m_InitThead_and_CS(void);
	// Called by d'tor
	void m_TerminateThread_and_CS(void);

	// Once the file is loaded, this does the actual 
	// copying to the VAO on the video card
	bool m_CopyLoadedModelToVAO(sModelDrawInfo& drawInfo,
		                        unsigned int shaderProgramID);

public:

	// This is used for the soft body (or anything else, like water)
	// Takes a mesh and copies it to another VAO, but the vertex array is DYNAMIC
	bool CloneMeshToDynamicVAO( std::string newMeshName, 
	                           sModelDrawInfo &drawInfo, 
	                           unsigned int shaderProgramID);
	bool UpdateDynamicMesh( std::string friendlyName, 
							// Will only look at the vertex info!
	                        sModelDrawInfo &updatedDrawInfo, 
	                        unsigned int shaderProgramID);

	// We don't want to return an int, likely
	bool FindDrawInfoByModelName(std::string filename,
								 sModelDrawInfo &drawInfo);

	std::string getLastError(bool bAndClear = true);

	// More general triangle information
	// (i.e. not coupled to the shader info
	//  that's in the sModelDrawInfo)
	struct sTriangle
	{
		glm::vec3 vertices[3];
		glm::vec3 normal;
		// Maybe other things?
	};
	// Returns false if didn't find mesh
	bool getTriangleMeshInfo(std::string meshName,
	                         std::vector<cVAOManager::sTriangle> &vecTriangles);

private:


	// 
	void m_LockModelMap(void);
//	bool isModelMapLocked(void);
//	bool m_bMapIsLocked = false;
	std::map< std::string /*model name*/,
		      sModelDrawInfo /* info needed to draw*/ >
		m_map_ModelName_to_VAOID;
	void m_UnlockModelMap(void);

};

#endif	// _cVAOManager_HG_
