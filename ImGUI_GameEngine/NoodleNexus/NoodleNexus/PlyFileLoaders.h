#ifndef _PlyFileLoaders_HG_
#define _PlyFileLoaders_HG_

#include <string>

// We'll make a struct that has ALL the things we need.

struct sPlyVertex
{
    float x, y, z, confidence, intensity;
};
//
// and 
// 
// 3 572 584 1040 
struct sTriangle
{
    unsigned int vertIndex_0;
    unsigned int vertIndex_1;
    unsigned int vertIndex_2;
};

struct s3DFileData
{
	std::string fileName;
	int numberOfVertices = 0;
	int numberOfTriangles = 0;
    sPlyVertex* pPlyVertices = NULL;        // nullptr
    sTriangle* pPlyTriangles = NULL;        // mullprt
};

// note the '&' symbol so we are passing this structure
//  by reference, not by value
bool ReadPlyModelFromFile_xyz_ci(s3DFileData& allFileInfo);

bool ReadPlyModelFromFile_xyz(s3DFileData& allFileInfo);

bool ReadPlyModelFromFile_FANCY(s3DFileData& allFileInfo);

#endif
