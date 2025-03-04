#include "cVAOManager.h"

#include "../GLCommon.h"	// All the GLFW, etc. (OpenGL stuff)

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <vector>

#include <sstream>

#include <fstream>

sModelDrawInfo::sModelDrawInfo()
{
	this->VAO_ID = 0;

	this->VertexBufferID = 0;
	this->VertexBuffer_Start_Index = 0;
	this->numberOfVertices = 0;

	this->IndexBufferID = 0;
	this->IndexBuffer_Start_Index = 0;
	this->numberOfIndices = 0;
	this->numberOfTriangles = 0;

	// The "local" (i.e. "CPU side" temporary array)
	this->pVertices = 0;	// or NULL
	this->pIndices = 0;		// or NULL

	this->centreXYZ = glm::vec3(0.0f);
	this->extenXYZ = glm::vec3(0.0f);
	this->maxXYZ = this->minXYZ = glm::vec3(0.0f);

	// You could store the max and min values of the 
	//  vertices here (determined when you load them):
//	glm::vec3 maxValues;
//	glm::vec3 minValues;

//	scale = 5.0/maxExtent;		-> 5x5x5
//	float maxExtent;

	return;
}


cVAOManager::cVAOManager()
{
	this->m_InitThead_and_CS();
}

cVAOManager::~cVAOManager()
{
	this->m_TerminateThread_and_CS();
}

// Takes:
// * 
// Returns: true if the file is loaded
bool cVAOManager::m_readPlyFile_XYZ(sModelDrawInfo& modelDrawInfo)
{

	// ************************************************************
		// Read the top of the file to get some info.

		// Read all the text until I get to the word "vertex"
//    std::ifstream plyFile("assets/models/bun_zipper_res3.ply");
	std::ifstream plyFile(modelDrawInfo.meshName);     // May also see .c_str() "c style string, char*"

	if ( ! plyFile.is_open()) 
	{
		return false;
	}

	std::string token = "";

	if (!plyFile.is_open())
	{
		return false;
	}

	//element vertex 1889
	while (token != "vertex")
	{
		plyFile >> token;
	};
	// Next info is the number of vertices
	//int numberOfVertices = 0;
	plyFile >> modelDrawInfo.numberOfVertices;


	//element face 3851
	while (token != "face")
	{
		plyFile >> token;
	};
	// Next info is the number of vertices
	//int numberOfTriangles = 0;
	plyFile >> modelDrawInfo.numberOfTriangles;

	//end_header
	// -0.0369122 0.127512 0.00276757 0.850855 0.5 
	while (token != "end_header")
	{
		plyFile >> token;
	};


	// 
//    std::cout << numberOfVertices << std::endl;
//    std::cout << numberOfTriangles << std::endl;

	//property float x
	//property float y
	//property float z

	//struct sPlyVertex
	//{
	//    float x, y, z, confidence, intensity;
	//};
	////
	//// and 
	//// 
	//// 3 572 584 1040 
	//struct sTriangle
	//{
	//    unsigned int vertIndex_0;
	//    unsigned int vertIndex_1;
	//    unsigned int vertIndex_2;
	//};

	// Load the data from the file
//    sPlyVertex* pPlyVertices = new sPlyVertex[numberOfVertices];
//	modelDrawInfo.pVertices = new sVertex_SHADER_FORMAT_xyz_rgb[modelDrawInfo.numberOfVertices];
//	modelDrawInfo.pVertices = new sVertex_SHADER_FORMAT_xyz_rgb_N[modelDrawInfo.numberOfVertices];
	modelDrawInfo.pVertices = new sVertex_SHADER_FORMAT_xyz_rgb_N_UV[modelDrawInfo.numberOfVertices];

	// end_header
	// -0.0369122 0.127512 0.00276757 0.850855 0.5
	for (unsigned index = 0; index != modelDrawInfo.numberOfVertices; index++)
	{
		plyFile >> modelDrawInfo.pVertices[index].x;
		plyFile >> modelDrawInfo.pVertices[index].y;
		plyFile >> modelDrawInfo.pVertices[index].z;

		// Set all the vertices to white (1,1,1)
		modelDrawInfo.pVertices->r = 1.0f;
		modelDrawInfo.pVertices->g = 1.0f;
		modelDrawInfo.pVertices->b = 1.0f;


		// Only has xyz, so stop here
//        plyFile >> allFileInfo.pPlyVertices[index].confidence;
//        plyFile >> allFileInfo.pPlyVertices[index].intensity;
//		modelDrawInfo.pVertices[index].confidence = 0;
//		modelDrawInfo.pVertices[index].intensity = 0;
	}

	// Load the triangle info from the file
	struct sPlyFileTriangle
	{
		unsigned int vertIndex_0;
		unsigned int vertIndex_1;
		unsigned int vertIndex_2;
	};
//    sTriangle* pPlyTriangles = new sTriangle[numberOfTriangles];
	sPlyFileTriangle* pPlyFileTriangles = new sPlyFileTriangle[modelDrawInfo.numberOfTriangles];
	for (unsigned int index = 0; index != modelDrawInfo.numberOfTriangles; index++)
	{
		// 3 737 103 17 
		int discard = 0;
		plyFile >> discard;
		plyFile >> pPlyFileTriangles[index].vertIndex_0;
		plyFile >> pPlyFileTriangles[index].vertIndex_1;
		plyFile >> pPlyFileTriangles[index].vertIndex_2;
	}

	// Copy the triangle data to a 1D array...
	modelDrawInfo.numberOfIndices = modelDrawInfo.numberOfTriangles * 3;

	modelDrawInfo.pIndices = new unsigned int[modelDrawInfo.numberOfIndices];

	unsigned int index = 0;
	for (unsigned int triIndex = 0; triIndex != modelDrawInfo.numberOfTriangles; triIndex++)
	{
		modelDrawInfo.pIndices[index + 0] = pPlyFileTriangles[triIndex].vertIndex_0;
		modelDrawInfo.pIndices[index + 1] = pPlyFileTriangles[triIndex].vertIndex_1;
		modelDrawInfo.pIndices[index + 2] = pPlyFileTriangles[triIndex].vertIndex_2;
		index += 3;
	}


	return true;
}

// Takes:
// * 
// Returns: true if the file is loaded
bool cVAOManager::m_readPlyFile_XYZ_Normal(sModelDrawInfo& modelDrawInfo)
{

	// ************************************************************
		// Read the top of the file to get some info.

//	ply
//	format ascii 1.0
//	comment VCGLIB generated
//	element vertex 53903
//	property float x
//	property float y
//	property float z
//	property float nx
//	property float ny
//	property float nz
//	element face 30320

		// Read all the text until I get to the word "vertex"
//    std::ifstream plyFile("assets/models/bun_zipper_res3.ply");
	std::ifstream plyFile(modelDrawInfo.meshName);     // May also see .c_str() "c style string, char*"

	if (!plyFile.is_open())
	{
		return false;
	}

	std::string token = "";

	if (!plyFile.is_open())
	{
		return false;
	}

	//element vertex 1889
	while (token != "vertex")
	{
		plyFile >> token;
	};
	// Next info is the number of vertices
	//int numberOfVertices = 0;
	plyFile >> modelDrawInfo.numberOfVertices;


	//element face 3851
	while (token != "face")
	{
		plyFile >> token;
	};
	// Next info is the number of vertices
	//int numberOfTriangles = 0;
	plyFile >> modelDrawInfo.numberOfTriangles;

	//end_header
	// -0.0369122 0.127512 0.00276757 0.850855 0.5 
	while (token != "end_header")
	{
		plyFile >> token;
	};



//	modelDrawInfo.pVertices = new sVertex_SHADER_FORMAT_xyz_rgb_N[modelDrawInfo.numberOfVertices];
	modelDrawInfo.pVertices = new sVertex_SHADER_FORMAT_xyz_rgb_N_UV[modelDrawInfo.numberOfVertices];

	// end_header
	// -0.0369122 0.127512 0.00276757 0.850855 0.5
	for (unsigned index = 0; index != modelDrawInfo.numberOfVertices; index++)
	{
		plyFile >> modelDrawInfo.pVertices[index].x;
		plyFile >> modelDrawInfo.pVertices[index].y;
		plyFile >> modelDrawInfo.pVertices[index].z;

		plyFile >> modelDrawInfo.pVertices[index].nx;
		plyFile >> modelDrawInfo.pVertices[index].ny;
		plyFile >> modelDrawInfo.pVertices[index].nz;

		// Set all the vertices to white (1,1,1)
		modelDrawInfo.pVertices->r = 1.0f;
		modelDrawInfo.pVertices->g = 1.0f;
		modelDrawInfo.pVertices->b = 1.0f;

		// There are no UVs in the file, so set them to 0.0
		modelDrawInfo.pVertices[index].u = 0.0f;
		modelDrawInfo.pVertices[index].v = 0.0f;



		// Only has xyz, so stop here
//        plyFile >> allFileInfo.pPlyVertices[index].confidence;
//        plyFile >> allFileInfo.pPlyVertices[index].intensity;
//		modelDrawInfo.pVertices[index].confidence = 0;
//		modelDrawInfo.pVertices[index].intensity = 0;
	}

	// Load the triangle info from the file
	struct sPlyFileTriangle
	{
		unsigned int vertIndex_0;
		unsigned int vertIndex_1;
		unsigned int vertIndex_2;
	};
	//    sTriangle* pPlyTriangles = new sTriangle[numberOfTriangles];
	sPlyFileTriangle* pPlyFileTriangles = new sPlyFileTriangle[modelDrawInfo.numberOfTriangles];
	for (unsigned int index = 0; index != modelDrawInfo.numberOfTriangles; index++)
	{
		// 3 737 103 17 
		int discard = 0;
		plyFile >> discard;
		plyFile >> pPlyFileTriangles[index].vertIndex_0;
		plyFile >> pPlyFileTriangles[index].vertIndex_1;
		plyFile >> pPlyFileTriangles[index].vertIndex_2;
	}

	// Copy the triangle data to a 1D array...
	modelDrawInfo.numberOfIndices = modelDrawInfo.numberOfTriangles * 3;

	modelDrawInfo.pIndices = new unsigned int[modelDrawInfo.numberOfIndices];

	unsigned int index = 0;
	for (unsigned int triIndex = 0; triIndex != modelDrawInfo.numberOfTriangles; triIndex++)
	{
		modelDrawInfo.pIndices[index + 0] = pPlyFileTriangles[triIndex].vertIndex_0;
		modelDrawInfo.pIndices[index + 1] = pPlyFileTriangles[triIndex].vertIndex_1;
		modelDrawInfo.pIndices[index + 2] = pPlyFileTriangles[triIndex].vertIndex_2;
		index += 3;
	}


	return true;
}

// Returns: true if the file is loaded
bool cVAOManager::m_readPlyFile_XYZ_Normal_UV(sModelDrawInfo& modelDrawInfo)
{

	// ************************************************************
		// Read the top of the file to get some info.

	//	ply
	//	format ascii 1.0
	//	comment VCGLIB generated
	//	element vertex 53903
	//	property float x
	//	property float y
	//	property float z
	//	property float nx
	//	property float ny
	//	property float nz
	//	property float texture_u
	//	property float texture_v
	//	element face 30320
	//	property list uchar int vertex_indices
	//	end_header

		// Read all the text until I get to the word "vertex"
//    std::ifstream plyFile("assets/models/bun_zipper_res3.ply");
	std::ifstream plyFile(modelDrawInfo.meshName);     // May also see .c_str() "c style string, char*"

	if (!plyFile.is_open())
	{
		return false;
	}

	std::string token = "";

	if (!plyFile.is_open())
	{
		return false;
	}

	//element vertex 1889
	while (token != "vertex")
	{
		plyFile >> token;
	};
	// Next info is the number of vertices
	//int numberOfVertices = 0;
	plyFile >> modelDrawInfo.numberOfVertices;


	//element face 3851
	while (token != "face")
	{
		plyFile >> token;
	};
	// Next info is the number of vertices
	//int numberOfTriangles = 0;
	plyFile >> modelDrawInfo.numberOfTriangles;

	//end_header
	// -0.0369122 0.127512 0.00276757 0.850855 0.5 
	while (token != "end_header")
	{
		plyFile >> token;
	};



	//	modelDrawInfo.pVertices = new sVertex_SHADER_FORMAT_xyz_rgb_N[modelDrawInfo.numberOfVertices];
	modelDrawInfo.pVertices = new sVertex_SHADER_FORMAT_xyz_rgb_N_UV[modelDrawInfo.numberOfVertices];

	// end_header
	// -0.0369122 0.127512 0.00276757 0.850855 0.5
	for (unsigned index = 0; index != modelDrawInfo.numberOfVertices; index++)
	{
		plyFile >> modelDrawInfo.pVertices[index].x;
		plyFile >> modelDrawInfo.pVertices[index].y;
		plyFile >> modelDrawInfo.pVertices[index].z;

		plyFile >> modelDrawInfo.pVertices[index].nx;
		plyFile >> modelDrawInfo.pVertices[index].ny;
		plyFile >> modelDrawInfo.pVertices[index].nz;

		// Set all the vertices to white (1,1,1)
		modelDrawInfo.pVertices->r = 1.0f;
		modelDrawInfo.pVertices->g = 1.0f;
		modelDrawInfo.pVertices->b = 1.0f;

		// Load the texture coordinates, too
		plyFile >> modelDrawInfo.pVertices[index].u;
		plyFile >> modelDrawInfo.pVertices[index].v;


		// Only has xyz, so stop here
//        plyFile >> allFileInfo.pPlyVertices[index].confidence;
//        plyFile >> allFileInfo.pPlyVertices[index].intensity;
//		modelDrawInfo.pVertices[index].confidence = 0;
//		modelDrawInfo.pVertices[index].intensity = 0;
	}

	// Load the triangle info from the file
	struct sPlyFileTriangle
	{
		unsigned int vertIndex_0;
		unsigned int vertIndex_1;
		unsigned int vertIndex_2;
	};
	//    sTriangle* pPlyTriangles = new sTriangle[numberOfTriangles];
	sPlyFileTriangle* pPlyFileTriangles = new sPlyFileTriangle[modelDrawInfo.numberOfTriangles];
	for (unsigned int index = 0; index != modelDrawInfo.numberOfTriangles; index++)
	{
		// 3 737 103 17 
		int discard = 0;
		plyFile >> discard;
		plyFile >> pPlyFileTriangles[index].vertIndex_0;
		plyFile >> pPlyFileTriangles[index].vertIndex_1;
		plyFile >> pPlyFileTriangles[index].vertIndex_2;
	}

	// Copy the triangle data to a 1D array...
	modelDrawInfo.numberOfIndices = modelDrawInfo.numberOfTriangles * 3;

	modelDrawInfo.pIndices = new unsigned int[modelDrawInfo.numberOfIndices];

	unsigned int index = 0;
	for (unsigned int triIndex = 0; triIndex != modelDrawInfo.numberOfTriangles; triIndex++)
	{
		modelDrawInfo.pIndices[index + 0] = pPlyFileTriangles[triIndex].vertIndex_0;
		modelDrawInfo.pIndices[index + 1] = pPlyFileTriangles[triIndex].vertIndex_1;
		modelDrawInfo.pIndices[index + 2] = pPlyFileTriangles[triIndex].vertIndex_2;
		index += 3;
	}


	return true;
}



bool cVAOManager::m_CopyLoadedModelToVAO(
	sModelDrawInfo& drawInfo,
	unsigned int shaderProgramID)
{
	//
	// 	// Create a VAO (Vertex Array Object), which will 
		//	keep track of all the 'state' needed to draw 
		//	from this buffer...

		// Ask OpenGL for a new buffer ID...
	glGenVertexArrays(1, &(drawInfo.VAO_ID));
	// "Bind" this buffer:
	// - aka "make this the 'current' VAO buffer
	glBindVertexArray(drawInfo.VAO_ID);

	// Now ANY state that is related to vertex or index buffer
	//	and vertex attribute layout, is stored in the 'state' 
	//	of the VAO... 


	// NOTE: OpenGL error checks have been omitted for brevity
	//	glGenBuffers(1, &vertex_buffer);
	glGenBuffers(1, &(drawInfo.VertexBufferID));

	//	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, drawInfo.VertexBufferID);
	// sVert vertices[3]
	//	glBufferData( GL_ARRAY_BUFFER, 
	//				  sizeof(sVertex_SHADER_FORMAT_xyz_rgb) * drawInfo.numberOfVertices,	// ::g_NumberOfVertsToDraw,	// sizeof(vertices), 
	//				  (GLvoid*) drawInfo.pVertices,							// pVertices,			//vertices, 
	//				  GL_STATIC_DRAW );
	//	glBufferData( GL_ARRAY_BUFFER, 
	//				  sizeof(sVertex_SHADER_FORMAT_xyz_rgb_N) * drawInfo.numberOfVertices,	// ::g_NumberOfVertsToDraw,	// sizeof(vertices), 
	//				  (GLvoid*) drawInfo.pVertices,							// pVertices,			//vertices, 
	//				  GL_STATIC_DRAW );
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(sVertex_SHADER_FORMAT_xyz_rgb_N_UV) * drawInfo.numberOfVertices,	// ::g_NumberOfVertsToDraw,	// sizeof(vertices), 
		(GLvoid*)drawInfo.pVertices,							// pVertices,			//vertices, 
		GL_STATIC_DRAW);

	// Copy the index buffer into the video card, too
	// Create an index buffer.
	glGenBuffers(1, &(drawInfo.IndexBufferID));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.IndexBufferID);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER,			// Type: Index element array
		sizeof(unsigned int) * drawInfo.numberOfIndices,
		(GLvoid*)drawInfo.pIndices,
		GL_STATIC_DRAW);

	// Set the vertex attributes.
	// ************************************************************
	// 	   SPECIFIC TO THE SHADER.
	// 	   If the shader changes or the vertex layout changes,
	// 		you have to change this part...
	// ************************************************************
	GLint vpos_location = glGetAttribLocation(shaderProgramID, "vPos");			// in vec3 vPos;
	GLint vcol_location = glGetAttribLocation(shaderProgramID, "vCol");			// in vec3 vCol;
	GLint vnorm_location = glGetAttribLocation(shaderProgramID, "vNormal");		// in vec3 vNormal;
	GLint vUV_location = glGetAttribLocation(shaderProgramID, "vUV");			// in vec2 vUV;


	//struct sVertex_SHADER_FORMAT_xyz_rgb_XXXXXXXXX
	//{
	//	float x, y, z;
	//	float r, g, b;
	//  float ny, ny, nz;	// When we added normals
	//  float u, v;			// When we added texture coords
	//};

	// Set the vertex attributes for this shader
	glEnableVertexAttribArray(vpos_location);	// vPos
	glVertexAttribPointer(vpos_location,
		3,		// vPos
		GL_FLOAT, GL_FALSE,
		//						   sizeof(sVertex_SHADER_FORMAT_xyz_rgb),	//  sizeof(float) * 6,		// Stride
		//						   sizeof(sVertex_SHADER_FORMAT_xyz_rgb_N),	//  sizeof(float) * 6,		// Stride
		sizeof(sVertex_SHADER_FORMAT_xyz_rgb_N_UV),	//  sizeof(float) * 6,		// Stride
		//						   ( void* )offsetof(sVertex_SHADER_FORMAT_xyz_rgb, x) );				// Offset
		//						   ( void* )offsetof(sVertex_SHADER_FORMAT_xyz_rgb_N, x) );				// Offset
		(void*)offsetof(sVertex_SHADER_FORMAT_xyz_rgb_N_UV, x));				// Offset

	glEnableVertexAttribArray(vcol_location);	// vCol
	glVertexAttribPointer(vcol_location,
		3,		// vCol
		GL_FLOAT, GL_FALSE,
		//		                   sizeof(sVertex_SHADER_FORMAT_xyz_rgb),						// sizeof(float) * 6,
		//		                   sizeof(sVertex_SHADER_FORMAT_xyz_rgb_N),			//( void* )( sizeof(float) * 3 ));
		sizeof(sVertex_SHADER_FORMAT_xyz_rgb_N_UV),			//( void* )( sizeof(float) * 3 ));
		//		                   sizeof(sVertex_SHADER_FORMAT_xyz_rgb),						// sizeof(float) * 6,
		//		                   (void*)offsetof(sVertex_SHADER_FORMAT_xyz_rgb_N, r));			//( void* )( sizeof(float) * 3 ));
		(void*)offsetof(sVertex_SHADER_FORMAT_xyz_rgb_N_UV, r));			//( void* )( sizeof(float) * 3 ));

	glEnableVertexAttribArray(vnorm_location);	// vNormal
	glVertexAttribPointer(vnorm_location,
		3,		// vNormal
		GL_FLOAT, GL_FALSE,
		//	                       sizeof(sVertex_SHADER_FORMAT_xyz_rgb_N),			//( void* )( sizeof(float) * 3 ));
		sizeof(sVertex_SHADER_FORMAT_xyz_rgb_N_UV),			//( void* )( sizeof(float) * 3 ));
		//		                   (void*)offsetof(sVertex_SHADER_FORMAT_xyz_rgb_N, nx));			//( void* )( sizeof(float) * 3 ));
		(void*)offsetof(sVertex_SHADER_FORMAT_xyz_rgb_N_UV, nx));			//( void* )( sizeof(float) * 3 ));

	glEnableVertexAttribArray(vUV_location);	// vUV
	glVertexAttribPointer(vUV_location,
		2,		// in vec2 vUV;
		GL_FLOAT, GL_FALSE,
		sizeof(sVertex_SHADER_FORMAT_xyz_rgb_N_UV),
		(void*)offsetof(sVertex_SHADER_FORMAT_xyz_rgb_N_UV, u));


	// Now that all the parts are set up, set the VAO to zero
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(vpos_location);
	glDisableVertexAttribArray(vcol_location);
	glDisableVertexAttribArray(vnorm_location);
	glDisableVertexAttribArray(vUV_location);

	// Indicate that model is in VAO
	drawInfo.bIsCopiedToVAO = true;

	// Store the draw information into the map
	this->m_LockModelMap();
	this->m_map_ModelName_to_VAOID[drawInfo.meshName] = drawInfo;
	this->m_UnlockModelMap();
	//

	return true;
}

bool cVAOManager::LoadModelIntoVAO(
		std::string fileName, 
		sModelDrawInfo &drawInfo,
	    unsigned int shaderProgramID)

{
	// Load the model from file
	// (We do this here, since if we can't load it, there's 
	//	no point in doing anything else, right?)

	drawInfo.meshName = fileName;

	// TODO: Load the model from file
//	if (!readPlyFile_XYZ(drawInfo))
//	{
//		return false;
//	}
//	if (!readPlyFile_XYZ_Normal(drawInfo))
//	{
//		return false;
//	}
	if ( ! this->m_readPlyFile_XYZ_Normal_UV(drawInfo) )
	{
		return false;
	}
	// Calculate extents
	drawInfo.calculateExtents();

	drawInfo.bIsLoaded = true;

	// 
	// Model is loaded and the vertices and indices are in the drawInfo struct
	// 

	return this->m_CopyLoadedModelToVAO(drawInfo, shaderProgramID);

}


// We don't want to return an int, likely
bool cVAOManager::FindDrawInfoByModelName(
		std::string filename,
		sModelDrawInfo &drawInfo) 
{

	this->m_LockModelMap();

	std::map< std::string /*model name*/,
			sModelDrawInfo /* info needed to draw*/ >::iterator 
		itDrawInfo = this->m_map_ModelName_to_VAOID.find( filename );

	this->m_UnlockModelMap();

	// Find it? 
	if ( itDrawInfo == this->m_map_ModelName_to_VAOID.end() )
	{
		// Nope
		return false;
	}

	// We do have the file name, but has it been loaded, yet
	if ( ! itDrawInfo->second.bIsLoaded )
	{
		// We have no drawing information to share
		return false;
	}

	if ( ! itDrawInfo->second.bIsCopiedToVAO )
	{
		// It's loaded, but not in the VAO yet
		return false;
	}

	// Else we found the thing to draw
	// ...so 'return' that information
	drawInfo = itDrawInfo->second;
	return true;
}


//glm::vec3 maxXYZ, minXYZ, extenXYZ, centreXYZ;
void sModelDrawInfo::calculateExtents(void)
{
	// Are there any vertices?
	if (this->numberOfIndices == 0)
	{
		return;
	}

	// Assume the 1st index is the max and min
	this->minXYZ.x = this->pVertices[0].x;
	this->minXYZ.y = this->pVertices[0].y;
	this->minXYZ.z = this->pVertices[0].z;

	this->maxXYZ.x = this->pVertices[0].x;
	this->maxXYZ.y = this->pVertices[0].y;
	this->maxXYZ.z = this->pVertices[0].z;

	for (unsigned int index = 0; index != this->numberOfVertices; index++)
	{
		// Compare each axis
		if (this->pVertices[index].x < this->minXYZ.x)
		{
			this->minXYZ.x = this->pVertices[index].x;
		}
		if (this->pVertices[index].y < this->minXYZ.y)
		{
			this->minXYZ.y = this->pVertices[index].y;
		}
		if (this->pVertices[index].z < this->minXYZ.z)
		{
			this->minXYZ.z = this->pVertices[index].z;
		}
		// 
		if (this->pVertices[index].x > this->maxXYZ.x)
		{
			this->maxXYZ.x = this->pVertices[index].x;
		}
		if (this->pVertices[index].y > this->maxXYZ.y)
		{
			this->maxXYZ.y = this->pVertices[index].y;
		}
		if (this->pVertices[index].z > this->maxXYZ.z)
		{
			this->maxXYZ.z = this->pVertices[index].z;
		}
	}//for (unsigned int index

	this->extenXYZ.x = this->maxXYZ.x - this->minXYZ.x;
	this->extenXYZ.y = this->maxXYZ.y - this->minXYZ.y;
	this->extenXYZ.z = this->maxXYZ.z - this->minXYZ.z;
	
	// TODO: Anything else you need calculated

	return;
}


bool cVAOManager::getTriangleMeshInfo(
	std::string meshName,
	std::vector<sTriangle>& vecTriangles)
{
	sModelDrawInfo meshInfo;
	if (!this->FindDrawInfoByModelName(meshName, meshInfo))
	{
		// Can't find that mesh... 
		return false;
	}

	vecTriangles.clear();

	// Copy that meshInfo to the vecTriangle vector
	// 
	// Recal that the index information is a 1D array, 
	//	and that every 3 indices is one triangle
	// Then we look up the vertex infom from the index
	//  info. AND we'll have to calculate the normal, too
	// (which is the cross product of the vertex locations)
	// 
	// 	sVertex_SHADER_FORMAT_xyz_rgb_N* pVertices;	
	//	unsigned int* pIndices;
	//
	// Remember that the element (triangle) indexes are in sets of 3s
	// pIndices[0] & pIndices[1] & pIndices[2] are triangle 0
	// pIndices[3] & pIndices[4] & pIndices[5] are triangle 1
	// ... and so on
	for (unsigned int index = 0; index != meshInfo.numberOfIndices; index += 3)
	{
		unsigned int vertIndex0 = meshInfo.pIndices[index + 0];
		unsigned int vertIndex1 = meshInfo.pIndices[index + 1];
		unsigned int vertIndex2 = meshInfo.pIndices[index + 2];

		sTriangle curTriangle;
		curTriangle.vertices[0].x = meshInfo.pVertices[vertIndex0].x;
		curTriangle.vertices[0].y = meshInfo.pVertices[vertIndex0].y;
		curTriangle.vertices[0].z = meshInfo.pVertices[vertIndex0].z;

		curTriangle.vertices[1].x = meshInfo.pVertices[vertIndex1].x;
		curTriangle.vertices[1].y = meshInfo.pVertices[vertIndex1].y;
		curTriangle.vertices[1].z = meshInfo.pVertices[vertIndex1].z;

		curTriangle.vertices[2].x = meshInfo.pVertices[vertIndex2].x;
		curTriangle.vertices[2].y = meshInfo.pVertices[vertIndex2].y;
		curTriangle.vertices[2].z = meshInfo.pVertices[vertIndex2].z;

		// Calculate the normal
		// https://en.wikipedia.org/wiki/Cross_product

		curTriangle.normal =
			glm::cross(curTriangle.vertices[1] - curTriangle.vertices[0],
				       curTriangle.vertices[2] - curTriangle.vertices[0]);
		// Normalize it
		curTriangle.normal = glm::normalize(curTriangle.normal);

		vecTriangles.push_back(curTriangle);

	}//for (unsigned int index




	return true;
}
