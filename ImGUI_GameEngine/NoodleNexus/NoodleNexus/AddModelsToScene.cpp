#include "sharedThings.h"
#include <iostream>
#include <sstream>
#include "cBasicTextureManager/cBasicTextureManager.h"
#include "cViperFlagConnector.h"

extern std::vector<sMesh*> g_vecMeshesToDraw;
extern cPhysics* g_pPhysicEngine;
extern cVAOManager* g_pMeshManager;
extern cBasicTextureManager* g_pTextures;
cViperFlagConnector* g_pViperFlagConnector = NULL;



void AddModelsToScene(cVAOManager* pMeshManager, GLuint program)
{

    // Off screen quad
    sModelDrawInfo flat10x10Mesh;
    ::g_pMeshManager->LoadModelIntoVAO("assets/models/10x10_FlatPlane_for_VerletSoftBodyFlag_xyz_n_uv.ply",
        flat10x10Mesh, program);
    std::cout << flat10x10Mesh.numberOfVertices << " vertices loaded" << std::endl;
    //
    sMesh* pOffScreenViewMesh = new sMesh();
    pOffScreenViewMesh->modelFileName = "assets/models/10x10_FlatPlane_for_VerletSoftBodyFlag_xyz_n_uv.ply";
    pOffScreenViewMesh->uniqueFriendlyName = "WareHouseView";
    pOffScreenViewMesh->positionXYZ = glm::vec3(0.0f, -250.0f, 100.0f);
    pOffScreenViewMesh->rotationEulerXYZ = glm::vec3(0.0f);
    pOffScreenViewMesh->textures[0] = "dua-lipa-promo.bmp";
    pOffScreenViewMesh->blendRatio[0] = 1.0f;
    pOffScreenViewMesh->uniformScale = 10.0f;
    pOffScreenViewMesh->bDoNotLight = true;
    ::g_vecMeshesToDraw.push_back(pOffScreenViewMesh);




    // this is the object that the Lua script, etc. is going to handle
    {
        sMesh* pNewViper = new sMesh();
        pNewViper->modelFileName = "assets/models/Viper_MkVII_xyz_n_uv.ply";
        pNewViper->positionXYZ = glm::vec3(-10.0f, 10.0f, 0.0f);
        pNewViper->objectColourRGBA = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
        pNewViper->bOverrideObjectColour = true;
        pNewViper->uniqueFriendlyName = "New_Viper_Player";
        pNewViper->bIsVisible = true;
        pNewViper->uniformScale = 5.0f;
        pNewViper->textures[0] = "dirty-metal-texture_1048-4784.bmp";
        pNewViper->blendRatio[0] = 1.0f;

        ::g_vecMeshesToDraw.push_back(pNewViper);

        // Add a associated physics object to have the phsyics "move" this
        cPhysics::sPhysInfo* pViperPhysObject = new  cPhysics::sPhysInfo();
        pViperPhysObject->bDoesntMove = false;
        pViperPhysObject->position = pNewViper->positionXYZ;
        pViperPhysObject->velocity = glm::vec3(0.0f);
        pViperPhysObject->pAssociatedDrawingMeshInstance = pNewViper;
        g_pPhysicEngine->vecGeneralPhysicsObjects.push_back(pViperPhysObject);
    }



    // Load a few soft bodies (two flags and a bunny)
    {
        sModelDrawInfo softBodyFlagMesh;
        ::g_pMeshManager->LoadModelIntoVAO("assets/models/10x10_FlatPlane_for_VerletSoftBodyFlag_xyz_n_uv.ply",
            softBodyFlagMesh, program);
        std::cout << softBodyFlagMesh.numberOfVertices << " vertices loaded" << std::endl;

        sModelDrawInfo softBodyFlagMesh1;
        ::g_pMeshManager->LoadModelIntoVAO("assets/models/30x30_FlatPlane_for_VerletSoftBodyFlag_xyz_n_uv.ply",
            softBodyFlagMesh1, program);
        std::cout << softBodyFlagMesh1.numberOfVertices << " vertices loaded" << std::endl;

        sModelDrawInfo softBodyFlagMesh2;
        ::g_pMeshManager->LoadModelIntoVAO("assets/models/60x60_FlatPlane_for_VerletSoftBodyFlag_xyz_n_uv_82,944_faces.ply",
            softBodyFlagMesh2, program);
        std::cout << softBodyFlagMesh2.numberOfVertices << " vertices loaded" << std::endl;

        sModelDrawInfo softBodyTube;
        ::g_pMeshManager->LoadModelIntoVAO("assets/models/tube_xyz_n_uv.ply",
            softBodyTube, program);
        std::cout << softBodyTube.numberOfVertices << " vertices loaded" << std::endl;

        sModelDrawInfo softBodyFlagMesh3;
        ::g_pMeshManager->LoadModelIntoVAO("assets/models/60x60_FlatPlane_for_VerletSoftBodyFlag_xyz_n_uv_497,656_faces.ply",
            softBodyFlagMesh3, program);
        std::cout << softBodyFlagMesh3.numberOfVertices << " vertices loaded" << std::endl;

        sModelDrawInfo bun_zipper_res4_larger_Mesh;
        ::g_pMeshManager->LoadModelIntoVAO("assets/models/bun_zipper_res4_larger_for_soft_body.ply",
            bun_zipper_res4_larger_Mesh, program);
        std::cout << bun_zipper_res4_larger_Mesh.meshName << " : " << bun_zipper_res4_larger_Mesh.numberOfVertices << " vertices loaded" << std::endl;



        sModelDrawInfo softBodyBunnyMeshDrawInfo;
        ::g_pMeshManager->FindDrawInfoByModelName("assets/models/bun_zipper_res4_larger_for_soft_body.ply", softBodyBunnyMeshDrawInfo);
        ::g_pMeshManager->CloneMeshToDynamicVAO("SoftBodyBunnyMesh_01", softBodyBunnyMeshDrawInfo, program);
        //
       glm::mat4 matModel = glm::mat4(1.0f);
//       // Add transforms you'd like...
//
       std::string error;
       cSoftBodyVerlet* pSB_Bunny = ::g_pPhysicEngine->createSoftBodyFromMesh("SoftBodyBunnyMesh_01", matModel, error);
//
//       // Add a force (gravity) to this bunny
       pSB_Bunny->acceleration = glm::vec3(0.0f, -1.0f, 0.0f);

       pSB_Bunny->CreateRandomBracing(1000, 5.0f);


        // DEBUG
//        {
            sMesh* pSoftBodyBunny = new sMesh();
            pSoftBodyBunny->modelFileName = "SoftBodyBunnyMesh_01";
            pSoftBodyBunny->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
            pSoftBodyBunny->objectColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            pSoftBodyBunny->bOverrideObjectColour = true;
            pSoftBodyBunny->bIsWireframe = true;
            //            pCanadianFlag->rotationEulerXYZ = glm::vec3(0.0f);
            //            pCanadianFlag->rotationEulerXYZ.y = 180.0f;
            //pSoftBodyBunny->textures[0] = "Canadian_Flag_Texture.bmp";
            //pSoftBodyBunny->blendRatio[0] = 1.0f;
            //pSoftBodyBunny->uniformScale = 1.0f;
            //pSoftBodyBunny->bDoNotLight = true;
            ::g_vecMeshesToDraw.push_back(pSoftBodyBunny);
//
//
//
//
            sModelDrawInfo softBodyCanadianFlagMesh;
//            ::g_pMeshManager->FindDrawInfoByModelName("assets/models/10x10_FlatPlane_for_VerletSoftBodyFlag_xyz_n_uv.ply", softBodyCanadianFlagMesh);
            ::g_pMeshManager->FindDrawInfoByModelName("assets/models/30x30_FlatPlane_for_VerletSoftBodyFlag_xyz_n_uv.ply", softBodyCanadianFlagMesh);
//            ::g_pMeshManager->FindDrawInfoByModelName("assets/models/60x60_FlatPlane_for_VerletSoftBodyFlag_xyz_n_uv_82,944_faces.ply", softBodyCanadianFlagMesh);
//            ::g_pMeshManager->FindDrawInfoByModelName("assets/models/60x60_FlatPlane_for_VerletSoftBodyFlag_xyz_n_uv_497,656_faces.ply", softBodyCanadianFlagMesh);
//            ::g_pMeshManager->FindDrawInfoByModelName("assets/models/tube_xyz_n_uv.ply", softBodyCanadianFlagMesh);
            ::g_pMeshManager->CloneMeshToDynamicVAO("CanadaFlag_SoftBodyMesh", softBodyCanadianFlagMesh, program);


            sMesh* pCanadianFlag = new sMesh();
            //            pCanadianFlag->modelFileName = "Canadian_Flag_Mesh";
            pCanadianFlag->modelFileName = "CanadaFlag_SoftBodyMesh";
            pCanadianFlag->uniqueFriendlyName = "Canadian_Flag";

//            pCanadianFlag->positionXYZ = glm::vec3(0.0f, +30.0f, 0.0f);
//            pCanadianFlag->rotationEulerXYZ = glm::vec3(0.0f, 0.0f, -90.0f);

//            pCanadianFlag->objectColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//            pCanadianFlag->bOverrideObjectColour = true;
            pCanadianFlag->bIsWireframe = true;
            //            pCanadianFlag->rotationEulerXYZ = glm::vec3(0.0f);
            //            pCanadianFlag->rotationEulerXYZ.y = 180.0f;
            pCanadianFlag->textures[0] = "Canadian_Flag_Texture.bmp";
            pCanadianFlag->blendRatio[0] = 1.0f;
            pCanadianFlag->uniformScale = 1.0f;
            pCanadianFlag->bDoNotLight = true;
//            pCanadianFlag->bIsVisible = false;
            ::g_vecMeshesToDraw.push_back(pCanadianFlag);


            glm::mat4 matModelCF = glm::mat4(1.0f);
    //        matModelCF = pCanadianFlag->calcMatModel();

            cSoftBodyVerlet* pSB_CanadianFlag = ::g_pPhysicEngine->createSoftBodyFromMesh("CanadaFlag_SoftBodyMesh", matModelCF, error);
            pSB_CanadianFlag->acceleration = glm::vec3(0.0f, -3.0f, 0.0f);

            // ********************************************************
            // START OF: Viper with flag set up

            ::g_pViperFlagConnector = new cViperFlagConnector();

            sMesh* pViperPlayer = g_pFindMeshByFriendlyName("New_Viper_Player");
            ::g_pViperFlagConnector->setViperModel(pViperPlayer);

            ::g_pViperFlagConnector->setFlagSoftBody(pSB_CanadianFlag);

            ::g_pViperFlagConnector->ConnectViperToFlag();

            // END OF: Viper with flag set up
            // ********************************************************



           //  Add the sphere that they soft bodies are hitting. 
           //  NOTE: The collision is HARD CODED (in the ApplyCollision() method of cSoftBodyVerlet.cpp).
           //  This object is here to make the collision more visible:
            
            glm::vec3 sphereCentre = glm::vec3(-1.0f, -30.0f, 1.0f);
            float sphereRadius = 15.0f;
            
            sMesh* pSoftBodyCollisionSphere = new sMesh();
            pSoftBodyCollisionSphere->modelFileName = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
            pSoftBodyCollisionSphere->positionXYZ = sphereCentre;
            pSoftBodyCollisionSphere->uniformScale = sphereRadius;
            pSoftBodyCollisionSphere->textures[0] = "Grey_Brick_Wall_Texture.bmp";
            pSoftBodyCollisionSphere->blendRatio[0] = 1.0f;
            ::g_vecMeshesToDraw.push_back(pSoftBodyCollisionSphere);

            sMesh* pSoftBodyCollisionSphereWireFrame = new sMesh();
            pSoftBodyCollisionSphereWireFrame->modelFileName = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
            pSoftBodyCollisionSphereWireFrame->positionXYZ = pSoftBodyCollisionSphere->positionXYZ;
            pSoftBodyCollisionSphereWireFrame->bIsWireframe = true;
            pSoftBodyCollisionSphereWireFrame->textures[0] = "solid_black.bmp";
            pSoftBodyCollisionSphereWireFrame->blendRatio[0] = 1.0f;
            pSoftBodyCollisionSphereWireFrame->bDoNotLight = true;
            pSoftBodyCollisionSphereWireFrame->uniformScale = pSoftBodyCollisionSphere->uniformScale * 1.001f;
            ::g_vecMeshesToDraw.push_back(pSoftBodyCollisionSphereWireFrame);

            //sModelDrawInfo softBodyCanadianFlagMesh;
            //::g_pMeshManager->FindDrawInfoByModelName("assets/models/10x10_FlatPlane_for_VerletSoftBodyFlag_xyz_n_uv.ply", softBodyCanadianFlagMesh);
            //::g_pMeshManager->CloneMeshToDynamicVAO("ChineseFlag_SoftBodyMesh", softBodyBunnyMeshDrawInfo, program);
            //sMesh* pChineseFlag = new sMesh();
            //pChineseFlag->modelFileName = "ChineseFlag_SoftBodyMesh";
            //pChineseFlag->positionXYZ = glm::vec3(50.0f, 0.0f, 200.0f);
            //pChineseFlag->objectColourRGBA = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
            //pChineseFlag->bOverrideObjectColour = true;
            //pChineseFlag->bIsWireframe = true;
            //pChineseFlag->rotationEulerXYZ = glm::vec3(0.0f);
            //pChineseFlag->rotationEulerXYZ.y = 180.0f;
            //pChineseFlag->textures[0] = "Chinese_Flag_Texture.bmp";
            //pChineseFlag->blendRatio[0] = 1.0f;
            //pChineseFlag->uniformScale = 5.0f;
            //::g_vecMeshesToDraw.push_back(pChineseFlag);
//
//
//        }

        // Add a "ground" for the bunny to hit
        sMesh* pPlaneForSoftBodiesToHit = new sMesh();
        pPlaneForSoftBodiesToHit->modelFileName = "assets/models/Flat_Plane_xyz_N_uv.ply";
        pPlaneForSoftBodiesToHit->positionXYZ = glm::vec3(0.0f, -50.5f, 0.0f);
        pPlaneForSoftBodiesToHit->textures[0] = "Grey_Brick_Wall_Texture.bmp";
        pPlaneForSoftBodiesToHit->blendRatio[0] = 1.0f;
        pPlaneForSoftBodiesToHit->alphaTransparency = 0.8f;
//        pPlaneForSoftBodiesToHit->bIsVisible = false;
        ::g_vecMeshesToDraw.push_back(pPlaneForSoftBodiesToHit);


    }// ENDOF: Load a few soft bodies (two flags and a bunny)







    {
        sModelDrawInfo galacticaModel;
        ::g_pMeshManager->LoadModelIntoVAO_Async("assets/models/Battlestar_Galactica_Res_0_(444,087 faces)_xyz_n_uv (facing +z, up +y).ply",
            galacticaModel, program);
        std::cout << galacticaModel.meshName << ": " << galacticaModel.numberOfVertices << " vertices loaded" << std::endl;
    }

    {
        sModelDrawInfo cubeMinXYZ_at_OriginInfo;
        ::g_pMeshManager->LoadModelIntoVAO("assets/models/Cube_MinXYZ_at_Origin_xyz_n_uv.ply",
            cubeMinXYZ_at_OriginInfo, program);
        std::cout << cubeMinXYZ_at_OriginInfo.meshName << ": " << cubeMinXYZ_at_OriginInfo.numberOfVertices << " vertices loaded" << std::endl;
    }

    {
        sModelDrawInfo warehouseModel;
        //    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Warehouse_xyz_n.ply",
        ::g_pMeshManager->LoadModelIntoVAO_Async("assets/models/Warehouse_xyz_n_uv.ply",
            warehouseModel, program);
        std::cout << warehouseModel.numberOfVertices << " vertices loaded" << std::endl;
    }

    {
        sModelDrawInfo tankModel;
        //    pMeshManager->LoadModelIntoVAO("assets/models/Low_Poly_Tank_Model_3D_model.ply", 
        pMeshManager->LoadModelIntoVAO("assets/models/Low_Poly_Tank_Model_3D_model_xyz_n_uv.ply",
            tankModel, program);
        std::cout << tankModel.meshName << " : " << tankModel.numberOfVertices << " vertices loaded" << std::endl;
    }

    //sModelDrawInfo carModelInfo;
    //pMeshManager->LoadModelIntoVAO("assets/models/VintageRacingCar_xyz_only.ply", 
    //                               carModelInfo, program);
    //std::cout << carModelInfo.numberOfVertices << " vertices loaded" << std::endl;

    //sModelDrawInfo dragonModel;
    //pMeshManager->LoadModelIntoVAO("assets/models/Dragon 2.5Edited_xyz_only.ply", 
    //    dragonModel, program);
    //std::cout << dragonModel.numberOfVertices << " vertices loaded" << std::endl;

    {
        sModelDrawInfo terrainModel;
        //    pMeshManager->LoadModelIntoVAO("assets/models/Simple_MeshLab_terrain_xyz_only.ply", 
    //    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Simple_MeshLab_terrain_xyz_N.ply",
    //    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Simple_MeshLab_terrain_xyz_N_uv.ply",
        ::g_pMeshManager->LoadModelIntoVAO_Async("assets/models/Simple_MeshLab_terrain_x5_xyz_N_uv.ply",
            terrainModel, program);
        std::cout << terrainModel.numberOfVertices << " vertices loaded" << std::endl;
    }

    {
        sModelDrawInfo bunnyModel;
        //    ::g_pMeshManager->LoadModelIntoVAO("assets/models/bun_zipper_res2_10x_size_xyz_only.ply",
    //    ::g_pMeshManager->LoadModelIntoVAO("assets/models/bun_zipper_res2_10x_size_xyz_N_only.ply",
        ::g_pMeshManager->LoadModelIntoVAO("assets/models/bun_zipper_res2_10x_size_xyz_N_uv.ply",
            bunnyModel, program);
        std::cout << bunnyModel.numberOfVertices << " vertices loaded" << std::endl;
    }

    {
        sModelDrawInfo platPlaneDrawInfo;
        //    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Flat_Plane_xyz.ply", 
    //    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Flat_Plane_xyz_N.ply",
        ::g_pMeshManager->LoadModelIntoVAO("assets/models/Flat_Plane_xyz_N_uv.ply",
            platPlaneDrawInfo, program);
        std::cout << platPlaneDrawInfo.numberOfVertices << " vertices loaded" << std::endl;
    }

    {
        sModelDrawInfo sphereMesh;
        //    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Sphere_radius_1_xyz.ply",
        //::g_pMeshManager->LoadModelIntoVAO("assets/models/Sphere_radius_1_xyz_N.ply",
        ::g_pMeshManager->LoadModelIntoVAO("assets/models/Sphere_radius_1_xyz_N_uv.ply",

            sphereMesh, program);
        std::cout << sphereMesh.numberOfVertices << " vertices loaded" << std::endl;
    }

    {
        sModelDrawInfo sphereShadowMesh;
        //    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Sphere_radius_1_Flat_Shadow_xyz_N.ply",
        ::g_pMeshManager->LoadModelIntoVAO("assets/models/Sphere_radius_1_Flat_Shadow_xyz_N_uv.ply",
            sphereShadowMesh, program);
        std::cout << sphereShadowMesh.numberOfVertices << " vertices loaded" << std::endl;
    }

    {
        sModelDrawInfo newViperModelInfo;
        ::g_pMeshManager->LoadModelIntoVAO_Async("assets/models/Viper_MkVII_xyz_n_uv.ply",
            newViperModelInfo, program);
        std::cout << newViperModelInfo.numberOfVertices << " vertices loaded" << std::endl;
    }

    {
        sModelDrawInfo cheeseMesh;
        ::g_pMeshManager->LoadModelIntoVAO_Async("assets/models/Cheese_xyz_n_uv.ply",
            cheeseMesh, program);
        std::cout << cheeseMesh.numberOfVertices << " vertices loaded" << std::endl;
    }

//    // Add a bunch of bunny rabbits
//    float boxLimit = 500.0f;
//    float boxStep = 50.0f;
//    unsigned int ID_count = 0;
//    for (float x = -boxLimit; x <= boxLimit; x += boxStep)
//    {
//        for (float z = -(2.0f * boxLimit); z <= boxLimit; z += boxStep)
//        {
//            sMesh* pBunny = new sMesh();
//            //            pBunny->modelFileName = "assets/models/bun_zipper_res2_10x_size_xyz_only.ply";
//            //            pBunny->modelFileName = "assets/models/bun_zipper_res2_10x_size_xyz_N_only.ply";
//            pBunny->modelFileName = "assets/models/bun_zipper_res2_10x_size_xyz_N_uv.ply";
//            pBunny->positionXYZ = glm::vec3(x, -35.0f, z);
//            pBunny->uniformScale = 2.0f;
//            pBunny->objectColourRGBA
//                = glm::vec4(::g_getRandomFloat(0.0f, 1.0f),
//                    ::g_getRandomFloat(0.0f, 1.0f),
//                    ::g_getRandomFloat(0.0f, 1.0f),
//                    1.0f);
//            // Set some transparency
//            pBunny->alphaTransparency = ::g_getRandomFloat(0.25f, 1.0f);
//            //            pBunny->alphaTransparency = 0.0f;
//            std::stringstream ssName;
//            ssName << "Bunny_" << ID_count;
//            pBunny->uniqueFriendlyName = ssName.str();
//            ID_count++;
//
//            ::g_vecMeshesToDraw.push_back(pBunny);
//        }
//    }//for (float x = -boxLimit...



    // Place a bunny somewhere else in the scene
    sMesh* pBunny_15 = ::g_pFindMeshByFriendlyName("Bunny_15");
    if (pBunny_15)
    {
        pBunny_15->positionXYZ = glm::vec3(-50.0f, 15.0f, 30.0f);
        pBunny_15->rotationEulerXYZ.x = glm::radians(180.0f);
        pBunny_15->uniformScale = 10.0f;
    }
    // Place a bunny somewhere else in the scene
    sMesh* pBunny_27 = ::g_pFindMeshByFriendlyName("Bunny_27");
    if (pBunny_27)
    {
        pBunny_27->positionXYZ = glm::vec3(75.0f, 10.0f, -45.0f);
        pBunny_27->rotationEulerXYZ.x = glm::radians(180.0f);
        pBunny_27->uniformScale = 10.0f;
    }



    {
        sMesh* pGalactica = new sMesh();
        pGalactica->modelFileName = "assets/models/Battlestar_Galactica_Res_0_(444,087 faces)_xyz_n_uv (facing +z, up +y).ply";
        pGalactica->positionXYZ = glm::vec3(-25'000.0f, 0.0f, 0.0f);
        pGalactica->rotationEulerXYZ.y = 17.0f;
        pGalactica->rotationEulerXYZ.x = 23.0f;
        pGalactica->objectColourRGBA = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
        //pGalactica->bIsWireframe = true;
        pGalactica->bOverrideObjectColour = true;
        pGalactica->uniqueFriendlyName = "Galactica";
        //pGalactica->bDoNotLight = true;
        pGalactica->bIsVisible = true;
        pGalactica->uniformScale = 1.0f;
        //
        pGalactica->textures[0] = "Non-uniform concrete wall 0512-3-1024x1024.bmp";
        pGalactica->blendRatio[0] = 1.0f;

        ::g_vecMeshesToDraw.push_back(pGalactica);

        // This is just for testing to see if the xyz locations correctly map to a gridID and the other way around
        unsigned long long gridIndex = ::g_pPhysicEngine->calcBP_GridIndex(0.0f, 0.0f, 0.0f, 1000.0f); // 0, 0, 0
        glm::vec3 minXYZ = ::g_pPhysicEngine->calcBP_MinXYZ_FromID(gridIndex, 1000.0f);
        gridIndex = ::g_pPhysicEngine->calcBP_GridIndex(500.0f, 500.0f, 500.0f, 1000.0f);              // 0, 0, 0
        minXYZ = ::g_pPhysicEngine->calcBP_MinXYZ_FromID(gridIndex, 1000.0f);
        gridIndex = ::g_pPhysicEngine->calcBP_GridIndex(-500.0f, -500.0f, -500.0f, 1000.0f);           // 
        minXYZ = ::g_pPhysicEngine->calcBP_MinXYZ_FromID(gridIndex, 1000.0f);
        gridIndex = ::g_pPhysicEngine->calcBP_GridIndex(10.0f, 2500.0f, 10.0f, 1000.0f);               // 0, 2, 0
        minXYZ = ::g_pPhysicEngine->calcBP_MinXYZ_FromID(gridIndex, 1000.0f);
        gridIndex = ::g_pPhysicEngine->calcBP_GridIndex(2500.0f, 10.0f, 10.0f, 1000.0f);               // 2, 0, 0
        minXYZ = ::g_pPhysicEngine->calcBP_MinXYZ_FromID(gridIndex, 1000.0f);
        gridIndex = ::g_pPhysicEngine->calcBP_GridIndex(10.0f, 10.0f, 2500.0f, 1000.0f);               // 0, 0, 2
        minXYZ = ::g_pPhysicEngine->calcBP_MinXYZ_FromID(gridIndex, 1000.0f);
        gridIndex = ::g_pPhysicEngine->calcBP_GridIndex(8745.0f, 3723.0f, 2500.0f, 1000.0f);           // 8, 3, 2
        minXYZ = ::g_pPhysicEngine->calcBP_MinXYZ_FromID(gridIndex, 1000.0f);
        gridIndex = ::g_pPhysicEngine->calcBP_GridIndex(-8745.0f, -3723.0f, -2500.0f, 1000.0f);           // 8, 3, 2
        minXYZ = ::g_pPhysicEngine->calcBP_MinXYZ_FromID(gridIndex, 1000.0f);
        gridIndex = ::g_pPhysicEngine->calcBP_GridIndex(-999.0f, -999.0f, -999.0f, 1000.0f);           // -1, -1, -1
        minXYZ = ::g_pPhysicEngine->calcBP_MinXYZ_FromID(gridIndex, 1000.0f);



        // 1000x1000x1000 aabbs
        //::g_pPhysicEngine->initBroadPhaseGrid();
//        ::g_pPhysicEngine->generateBroadPhaseGrid(
//            "assets/models/Battlestar_Galactica_Res_0_(444,087 faces)_xyz_n_uv (facing +z, up +y).ply",
//            1000.0f,                            // AABB Cube region size
//            pGalactica->positionXYZ,
//            pGalactica->rotationEulerXYZ,
//            pGalactica->uniformScale);


        sMesh* pGalacticaWireframe = new sMesh();
        pGalacticaWireframe->modelFileName = "assets/models/Battlestar_Galactica_Res_0_(444,087 faces)_xyz_n_uv (facing +z, up +y).ply";
        pGalacticaWireframe->objectColourRGBA = glm::vec4(0.0f, 0.0f, 0.5f, 1.0f);
        pGalacticaWireframe->positionXYZ = pGalactica->positionXYZ;
        pGalacticaWireframe->rotationEulerXYZ = pGalactica->rotationEulerXYZ;
        pGalacticaWireframe->uniformScale = pGalactica->uniformScale;
        pGalacticaWireframe->bIsWireframe = true;
        pGalacticaWireframe->bOverrideObjectColour = true;
        pGalacticaWireframe->bDoNotLight = true;
        pGalacticaWireframe->bIsVisible = true;

        ::g_vecMeshesToDraw.push_back(pGalacticaWireframe);


        // Debug AABB shape
        sMesh* pAABBCube_MinAtOrigin = new sMesh();
        pAABBCube_MinAtOrigin->modelFileName = "assets/models/Cube_MinXYZ_at_Origin_xyz_n_uv.ply";
        pAABBCube_MinAtOrigin->bIsWireframe = true;
        pAABBCube_MinAtOrigin->objectColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        pAABBCube_MinAtOrigin->bOverrideObjectColour = true;
        pAABBCube_MinAtOrigin->bDoNotLight = true;
        pAABBCube_MinAtOrigin->bIsVisible = false;
        pAABBCube_MinAtOrigin->uniqueFriendlyName = "AABB_MinXYZ_At_Origin";

        ::g_vecMeshesToDraw.push_back(pAABBCube_MinAtOrigin);
    }

    {
        sMesh* pSkySphere = new sMesh();
        pSkySphere->modelFileName = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
        pSkySphere->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
        pSkySphere->objectColourRGBA = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
        //       pSkySphere->bIsWireframe = true;
        pSkySphere->bOverrideObjectColour = true;
        pSkySphere->uniformScale = 25.0f;
        pSkySphere->uniqueFriendlyName = "SkySphere";
        pSkySphere->textures[0] = "bad_bunny_1920x1080.bmp";
        pSkySphere->blendRatio[0] = 1.0f;
        pSkySphere->bIsVisible = false;
        ::g_vecMeshesToDraw.push_back(pSkySphere);
    }



    {
        //    ____                _            __                   _     
        //   |  _ \ ___ _ __   __| | ___ _ __ / / __ ___   ___  ___| |__  
        //   | |_) / _ \ '_ \ / _` |/ _ \ '__/ / '_ ` _ \ / _ \/ __| '_ \ 
        //   |  _ <  __/ | | | (_| |  __/ | / /| | | | | |  __/\__ \ | | |
        //   |_| \_\___|_| |_|\__,_|\___|_|/_/ |_| |_| |_|\___||___/_| |_|
        //                                                                
        sMesh* pWarehouse = new sMesh();
        //        pWarehouse->modelFileName = "assets/models/Warehouse_xyz_n.ply";
        pWarehouse->modelFileName = "assets/models/Warehouse_xyz_n_uv.ply";
        pWarehouse->positionXYZ = glm::vec3(-200.0f, 5.0f, 0.0f);
        pWarehouse->rotationEulerXYZ.y = -90.0f;
        pWarehouse->objectColourRGBA = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
        //pWarehouse->bIsWireframe = true;
        pWarehouse->bOverrideObjectColour = true;
        pWarehouse->uniqueFriendlyName = "Warehouse";
        //
        pWarehouse->textures[0] = "bad_bunny_1920x1080.bmp";

        ::g_vecMeshesToDraw.push_back(pWarehouse);

        //    ____  _               _                  _     _           _   
        //   |  _ \| |__  _   _ ___(_) ___ ___    ___ | |__ (_) ___  ___| |_ 
        //   | |_) | '_ \| | | / __| |/ __/ __|  / _ \| '_ \| |/ _ \/ __| __|
        //   |  __/| | | | |_| \__ \ | (__\__ \ | (_) | |_) | |  __/ (__| |_ 
        //   |_|   |_| |_|\__, |___/_|\___|___/  \___/|_.__// |\___|\___|\__|
        //                |___/                           |__/               
        ::g_pPhysicEngine->addTriangleMesh(
            "assets/models/Warehouse_xyz_n_uv.ply",
            pWarehouse->positionXYZ,
            pWarehouse->rotationEulerXYZ,
            pWarehouse->uniformScale);

    }

    {
        sMesh* pTerrain = new sMesh();
        pTerrain->modelFileName = "assets/models/Simple_MeshLab_terrain_x5_xyz_N_uv.ply";
        pTerrain->positionXYZ = glm::vec3(0.0f, -175.0f, 0.0f);
        pTerrain->uniqueFriendlyName = "Terrain";
        pTerrain->textures[0] = "Grey_Brick_Wall_Texture.bmp";
        pTerrain->blendRatio[0] = 1.0f;
        ::g_vecMeshesToDraw.push_back(pTerrain);



//        sMesh* pTerrainWireFrame = new sMesh();
//        pTerrainWireFrame->modelFileName = "assets/models/Simple_MeshLab_terrain_x5_xyz_N_uv.ply";
//        pTerrainWireFrame->positionXYZ = glm::vec3(0.0f, -173.8f, 0.0f);
//        pTerrainWireFrame->bDoNotLight = true;
//        pTerrainWireFrame->bOverrideObjectColour = true;
//        pTerrainWireFrame->objectColourRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
//        pTerrainWireFrame->uniqueFriendlyName = "Terrain_Wireframe";
//        pTerrainWireFrame->rotationEulerXYZ.y = 90.0f;
//        pTerrainWireFrame->bIsWireframe = true;
////        pTerrainWireFrame->textures[0] = "Grey_Brick_Wall_Texture.bmp";
////        pTerrainWireFrame->blendRatio[0] = 1.0f;
//        ::g_vecMeshesToDraw.push_back(pTerrainWireFrame);
    }

    {


        sMesh* pFlatPlane = new sMesh();
        pFlatPlane->modelFileName = "assets/models/Flat_Plane_xyz_N_uv.ply";
        pFlatPlane->positionXYZ = glm::vec3(0.0f, -5.5f, 0.0f);
        pFlatPlane->rotationEulerXYZ.y = 90.0f;
        pFlatPlane->objectColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        pFlatPlane->uniqueFriendlyName = "Ground";
        //
        pFlatPlane->textures[0] = "dua-lipa-promo.bmp";     // 1.0
        pFlatPlane->textures[1] = "Puzzle_parts.bmp";       // 0.0
        pFlatPlane->textures[2] = "shape-element-splattered-texture-stroke_1194-8223.bmp";
        pFlatPlane->textures[3] = "Grey_Brick_Wall_Texture.bmp";

        //        pFlatPlane->alphaTransparency = 0.5f;

        pFlatPlane->blendRatio[0] = 0.0f;
        pFlatPlane->blendRatio[1] = 1.0f;

        pFlatPlane->bIsVisible = false;

        //
        //        pFlatPlane->bIsWireframe = true;
        //        ::g_myMeshes[::g_NumberOfMeshesToDraw] = pFlatPlane;
        //        ::g_NumberOfMeshesToDraw++;
        ::g_vecMeshesToDraw.push_back(pFlatPlane);


        // Add the "ground" to the physcs
        cPhysics::sAABB* pAABBGround = new cPhysics::sAABB();
        pAABBGround->centreXYZ = pFlatPlane->positionXYZ;
        sModelDrawInfo planeMeshInfo;
        ::g_pMeshManager->FindDrawInfoByModelName(pFlatPlane->modelFileName, planeMeshInfo);

        // Manually enter the AABB info:
        pAABBGround->centreXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
        // How far from the centre the XYZ min and max are
        // This information is from the mesh we loaded
        // WARNING: We need to be careful about the scale
        pAABBGround->minXYZ.x = -100.0f;
        pAABBGround->maxXYZ.x = 100.0f;

        pAABBGround->minXYZ.z = -100.0f;
        pAABBGround->maxXYZ.z = 100.0f;

        pAABBGround->minXYZ.y = -1.0f;
        pAABBGround->maxXYZ.y = 1.0f;

        // Copy the physics object position from the initial mesh position
        pAABBGround->pPhysicInfo->position = pFlatPlane->positionXYZ;

        // Don't move this ground (skip integration step)
        pAABBGround->pPhysicInfo->bDoesntMove = true;

        pAABBGround->pPhysicInfo->pAssociatedDrawingMeshInstance = pFlatPlane;

        ::g_pPhysicEngine->vecAABBs.push_back(pAABBGround);
    }
    //    {
    //        sMesh* pFlatPlane = new sMesh();
    ////        pFlatPlane->modelFileName = "assets/models/Flat_Plane_xyz.ply";
    ////        pFlatPlane->modelFileName = "assets/models/Flat_Plane_xyz_N.ply";
    //        pFlatPlane->modelFileName = "assets/models/Flat_Plane_xyz_N_uv.ply";
    //        pFlatPlane->positionXYZ = glm::vec3(0.0f, -5.0f, 0.0f);
    //        pFlatPlane->bIsWireframe = true;
    //        pFlatPlane->uniformScale = 1.01f;
    //        pFlatPlane->objectColourRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    //
    //        ::g_vecMeshesToDraw.push_back(pFlatPlane);
    //    }



    {

        //    ____                _            __                   _     
        //   |  _ \ ___ _ __   __| | ___ _ __ / / __ ___   ___  ___| |__  
        //   | |_) / _ \ '_ \ / _` |/ _ \ '__/ / '_ ` _ \ / _ \/ __| '_ \ 
        //   |  _ <  __/ | | | (_| |  __/ | / /| | | | | |  __/\__ \ | | |
        //   |_| \_\___|_| |_|\__,_|\___|_|/_/ |_| |_| |_|\___||___/_| |_|
        //                                                                
        sMesh* pSphereMesh = new sMesh();
        //        pSphereMesh->modelFileName = "assets/models/Sphere_radius_1_xyz.ply";
        //        pSphereMesh->modelFileName = "assets/models/Sphere_radius_1_xyz_N.ply";
        pSphereMesh->modelFileName = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
        pSphereMesh->positionXYZ = glm::vec3(-15.0f, -3.0f, -20.0f);
        //pSphereMesh->bIsWireframe = true;
        pSphereMesh->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        pSphereMesh->uniqueFriendlyName = "Ball";

        //::g_myMeshes[::g_NumberOfMeshesToDraw] = pSphere;
        //::g_NumberOfMeshesToDraw++;
        ::g_vecMeshesToDraw.push_back(pSphereMesh);

        {
            sMesh* pSphereShadowMesh = new sMesh();
            //            pSphereShadowMesh->modelFileName = "assets/models/Sphere_radius_1_Flat_Shadow_xyz_N.ply";
            pSphereShadowMesh->modelFileName = "assets/models/Sphere_radius_1_Flat_Shadow_xyz_N_uv.ply";
            pSphereShadowMesh->positionXYZ = pSphereMesh->positionXYZ;
            pSphereShadowMesh->positionXYZ.y = -3.95f;  // JUST above the ground
            pSphereShadowMesh->objectColourRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            pSphereShadowMesh->uniqueFriendlyName = "Ball_Shadow";
            ::g_vecMeshesToDraw.push_back(pSphereShadowMesh);
        }


        //    ____  _               _                  _     _           _   
        //   |  _ \| |__  _   _ ___(_) ___ ___    ___ | |__ (_) ___  ___| |_ 
        //   | |_) | '_ \| | | / __| |/ __/ __|  / _ \| '_ \| |/ _ \/ __| __|
        //   |  __/| | | | |_| \__ \ | (__\__ \ | (_) | |_) | |  __/ (__| |_ 
        //   |_|   |_| |_|\__, |___/_|\___|___/  \___/|_.__// |\___|\___|\__|
        //                |___/                           |__/               
        // Add sphere
        cPhysics::sSphere* pSphereInfo = new cPhysics::sSphere();

        pSphereInfo->centre = glm::vec3(0.0f);  // Sphere's centre (i.e. an offset from the position)

        pSphereInfo->pPhysicInfo->position = pSphereMesh->positionXYZ;
        // HACK: We know this is 1.0 because...?
        // We could also have pulled that information from the mesh info
        pSphereInfo->radius = 1.0f;

        pSphereInfo->pPhysicInfo->velocity.y = 7.5f;

        // Set some x velocity
        pSphereInfo->pPhysicInfo->velocity.x = 1.0f;


        pSphereInfo->pPhysicInfo->acceleration.y = -3.0f;

        // Associate this drawing mesh to this physics object
        pSphereInfo->pPhysicInfo->pAssociatedDrawingMeshInstance = pSphereMesh;

        ::g_pPhysicEngine->vecSpheres.push_back(pSphereInfo);
    }


    for (unsigned int ballCount = 0; ballCount != 10; ballCount++)
    {
        //    ____                _            __                   _     
        //   |  _ \ ___ _ __   __| | ___ _ __ / / __ ___   ___  ___| |__  
        //   | |_) / _ \ '_ \ / _` |/ _ \ '__/ / '_ ` _ \ / _ \/ __| '_ \ 
        //   |  _ <  __/ | | | (_| |  __/ | / /| | | | | |  __/\__ \ | | |
        //   |_| \_\___|_| |_|\__,_|\___|_|/_/ |_| |_| |_|\___||___/_| |_|
        //                                                                
        sMesh* pSphereMesh = new sMesh();
        //        pSphereMesh->modelFileName = "assets/models/Sphere_radius_1_xyz.ply";
//        pSphereMesh->modelFileName = "assets/models/Sphere_radius_1_xyz_N.ply";
        pSphereMesh->modelFileName = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
        pSphereMesh->positionXYZ.x = ::g_getRandomFloat(-30.0f, 30.0f);
        pSphereMesh->positionXYZ.z = ::g_getRandomFloat(-30.0f, 30.0f);
        pSphereMesh->positionXYZ.y = ::g_getRandomFloat(0.0f, 40.0f);
        //pSphereMesh->bIsWireframe = true;
        pSphereMesh->objectColourRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        pSphereMesh->objectColourRGBA.r = ::g_getRandomFloat(0.0f, 1.0f);
        pSphereMesh->objectColourRGBA.g = ::g_getRandomFloat(0.0f, 1.0f);
        pSphereMesh->objectColourRGBA.b = ::g_getRandomFloat(0.0f, 1.0f);
        std::stringstream ssBallName;
        ssBallName << "Bouncy_" << ballCount;
        pSphereMesh->uniqueFriendlyName = ssBallName.str();

        //
        pSphereMesh->textures[0] = "Non-uniform concrete wall 0512-3-1024x1024.bmp";

        ::g_vecMeshesToDraw.push_back(pSphereMesh);

        //    ____  _               _                  _     _           _   
        //   |  _ \| |__  _   _ ___(_) ___ ___    ___ | |__ (_) ___  ___| |_ 
        //   | |_) | '_ \| | | / __| |/ __/ __|  / _ \| '_ \| |/ _ \/ __| __|
        //   |  __/| | | | |_| \__ \ | (__\__ \ | (_) | |_) | |  __/ (__| |_ 
        //   |_|   |_| |_|\__, |___/_|\___|___/  \___/|_.__// |\___|\___|\__|
        //                |___/                           |__/               
        // Add sphere
        cPhysics::sSphere* pSphereInfo = new cPhysics::sSphere();
        pSphereInfo->centre = glm::vec3(0.0f);  // Sphere's centre (i.e. an offset from the position)
        pSphereInfo->pPhysicInfo->position = pSphereMesh->positionXYZ;
        pSphereInfo->radius = 1.0f;
        pSphereInfo->pPhysicInfo->velocity.y = ::g_getRandomFloat(2.0f, 10.0f);
        pSphereInfo->pPhysicInfo->velocity.x = ::g_getRandomFloat(-5.0f, 5.0f);
        pSphereInfo->pPhysicInfo->velocity.z = ::g_getRandomFloat(-5.0f, 5.0f);
        pSphereInfo->pPhysicInfo->acceleration.y = -3.0f;
        pSphereInfo->pPhysicInfo->pAssociatedDrawingMeshInstance = pSphereMesh;
        ::g_pPhysicEngine->vecSpheres.push_back(pSphereInfo);
    }//for ( unsigned int ballCount


    return;
}

