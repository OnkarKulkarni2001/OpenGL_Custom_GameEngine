#include "PlyFileLoaders.h"

//#include <string>
#include <fstream>

// Takes:
// * 
// Returns: true if the file is loaded
bool ReadPlyModelFromFile_xyz_ci(s3DFileData& allFileInfo)
{

    // ************************************************************
        // Read the top of the file to get some info.

        // Read all the text until I get to the word "vertex"
//    std::ifstream plyFile("assets/models/bun_zipper_res3.ply");
    std::ifstream plyFile(allFileInfo.fileName);     // May also see .c_str() "c style string, char*"
    std::string token = "";

    if ( ! plyFile.is_open() )
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
    plyFile >> allFileInfo.numberOfVertices;


    //element face 3851
    while (token != "face")
    {
        plyFile >> token;
    };
    // Next info is the number of vertices
    //int numberOfTriangles = 0;
    plyFile >> allFileInfo.numberOfTriangles;

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
    //property float confidence
    //property float intensity
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
    allFileInfo.pPlyVertices = new sPlyVertex[allFileInfo.numberOfVertices];

    // end_header
    // -0.0369122 0.127512 0.00276757 0.850855 0.5
    for (unsigned index = 0; index != allFileInfo.numberOfVertices; index++)
    {
        plyFile >> allFileInfo.pPlyVertices[index].x;
        plyFile >> allFileInfo.pPlyVertices[index].y;
        plyFile >> allFileInfo.pPlyVertices[index].z;
        plyFile >> allFileInfo.pPlyVertices[index].confidence;
        plyFile >> allFileInfo.pPlyVertices[index].intensity;
    }

    // Load the triangle info from the file
//    sTriangle* pPlyTriangles = new sTriangle[numberOfTriangles];
    allFileInfo.pPlyTriangles = new sTriangle[allFileInfo.numberOfTriangles];
    for (unsigned int index = 0; index != allFileInfo.numberOfTriangles; index++)
    {
        // 3 737 103 17 
        int discard = 0;
        plyFile >> discard;
        plyFile >> allFileInfo.pPlyTriangles[index].vertIndex_0;
        plyFile >> allFileInfo.pPlyTriangles[index].vertIndex_1;
        plyFile >> allFileInfo.pPlyTriangles[index].vertIndex_2;
    }



    return true;
}





// Takes:
// * 
// Returns: true if the file is loaded
bool ReadPlyModelFromFile_xyz(s3DFileData& allFileInfo)
{

    // ************************************************************
        // Read the top of the file to get some info.

        // Read all the text until I get to the word "vertex"
//    std::ifstream plyFile("assets/models/bun_zipper_res3.ply");
    std::ifstream plyFile(allFileInfo.fileName);     // May also see .c_str() "c style string, char*"
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
    plyFile >> allFileInfo.numberOfVertices;


    //element face 3851
    while (token != "face")
    {
        plyFile >> token;
    };
    // Next info is the number of vertices
    //int numberOfTriangles = 0;
    plyFile >> allFileInfo.numberOfTriangles;

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
    allFileInfo.pPlyVertices = new sPlyVertex[allFileInfo.numberOfVertices];

    // end_header
    // -0.0369122 0.127512 0.00276757 0.850855 0.5
    for (unsigned index = 0; index != allFileInfo.numberOfVertices; index++)
    {
        plyFile >> allFileInfo.pPlyVertices[index].x;
        plyFile >> allFileInfo.pPlyVertices[index].y;
        plyFile >> allFileInfo.pPlyVertices[index].z;
        
        // Only has xyz, so stop here
//        plyFile >> allFileInfo.pPlyVertices[index].confidence;
//        plyFile >> allFileInfo.pPlyVertices[index].intensity;
        allFileInfo.pPlyVertices[index].confidence = 0;
        allFileInfo.pPlyVertices[index].intensity = 0;
    }

    // Load the triangle info from the file
//    sTriangle* pPlyTriangles = new sTriangle[numberOfTriangles];
    allFileInfo.pPlyTriangles = new sTriangle[allFileInfo.numberOfTriangles];
    for (unsigned int index = 0; index != allFileInfo.numberOfTriangles; index++)
    {
        // 3 737 103 17 
        int discard = 0;
        plyFile >> discard;
        plyFile >> allFileInfo.pPlyTriangles[index].vertIndex_0;
        plyFile >> allFileInfo.pPlyTriangles[index].vertIndex_1;
        plyFile >> allFileInfo.pPlyTriangles[index].vertIndex_2;
    }



    return true;
}