#include "cScene.h"
#include <iostream>
#include <fstream>

using namespace std;

void cScene::CreateScene() {
    std::string sceneFileName = "D:/Graphics1/GameEngine/sceneFile.txt";
    std::string token = "";

    ifstream sceneFile(sceneFileName);

    if (!sceneFile.is_open()) {
        cout << "Trying to open file: " << sceneFileName << endl;
        cout << "Unable to open file because it is already open!" << endl;
        return;
    }
    else {
        cout << "Scenefile open successful!" << endl;
    }

    // Taking input for number of meshes to load
    while (token != "number_of_meshes") {
        sceneFile >> token;
    }
    sceneFile >> numberOfMeshesToLoad;

    // Taking input for mesh paths
    while (token != "mesh_paths") {
        sceneFile >> token;
    }

    // Pushing back the mesh paths to the vector pathOfMeshesToLoad
    for (unsigned int indexOfMeshesToLoad = 0; indexOfMeshesToLoad != numberOfMeshesToLoad; indexOfMeshesToLoad++) {
        sceneFile >> meshPath;
        meshPath.erase(std::remove(meshPath.begin(), meshPath.end(), '"'), meshPath.end());
        for (char& ch : meshPath) {
            if (ch == '\\') {
                ch = '/';
            }
        }
        //pathOfMeshesToLoad.push_back(meshPath);
        cLoadModels plyModel;
        plyModel.LoadPlyModel(meshPath);
        pModels.push_back(plyModel);
    }

    // Taking input for mesh transforms
    while (token != "mesh_transforms") {
        sceneFile >> token;
    }

    int modelIndex = 0;
    for (unsigned int indexOfMeshTransforms = 0; indexOfMeshTransforms != numberOfMeshesToLoad; indexOfMeshTransforms++) { // Remember whenever you need to call transforms of any model you just need to pass their index!

        sceneFile >> pModels[modelIndex].pMeshTransform.x;
        sceneFile >> pModels[modelIndex].pMeshTransform.y;
        sceneFile >> pModels[modelIndex].pMeshTransform.z;

        sceneFile >> pModels[modelIndex].pMeshTransform.xScale;
        sceneFile >> pModels[modelIndex].pMeshTransform.yScale;
        sceneFile >> pModels[modelIndex].pMeshTransform.zScale;

        sceneFile >> pModels[modelIndex].pMeshTransform.xRotation;
        sceneFile >> pModels[modelIndex].pMeshTransform.yRotation;
        sceneFile >> pModels[modelIndex].pMeshTransform.zRotation;

        modelIndex++;
    }
}