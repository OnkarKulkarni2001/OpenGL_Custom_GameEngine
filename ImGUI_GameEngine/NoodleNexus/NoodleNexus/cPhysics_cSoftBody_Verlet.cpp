
#include "cPhysics.h"
//	#include "../sharedThings.h"


cSoftBodyVerlet* cPhysics::createSoftBodyFromMesh(std::string meshFriendlyName, glm::mat4 matModel, std::string & error)
{
    sModelDrawInfo softBodyMeshDrawingInfo;

    if ( ! this->m_pVAOManager->FindDrawInfoByModelName(meshFriendlyName, softBodyMeshDrawingInfo) )
    {
        error = "Can't find >" + meshFriendlyName + "< mesh";
        return NULL;
    }


    cSoftBodyVerlet* pNewSoftBody = new cSoftBodyVerlet();

    pNewSoftBody->matching_VAO_MeshName = meshFriendlyName;

    pNewSoftBody->CreateSoftBody(softBodyMeshDrawingInfo, matModel);

//    pNewSoftBody->CreateRandomBracing(20, 1.0f);

    // Add this soft body to the map of soft bodies
    this->m_MapSoftBodiesByName[meshFriendlyName] = pNewSoftBody;

    return pNewSoftBody;
}





cSoftBodyVerlet* cPhysics::pFindSoftBodyByFriendlyName(std::string friendlyName)
{
    std::map< std::string /*freindly name*/, cSoftBodyVerlet* >::iterator itSB 
        = this->m_MapSoftBodiesByName.find(friendlyName);

    // Found it? 
    if (itSB == this->m_MapSoftBodiesByName.end())
    {
        // NO, we didn't
        return NULL;
    }

    // return pointer to soft body
    return itSB->second;

}


// 
//	std::map< std::string /*freindly name*/, cSoftBody_Verlet* > m_MapSoftBodiesByName;


void cPhysics::updateSoftBodyMeshes(unsigned int shaderProgramID)
{
    for (std::map< std::string, cSoftBodyVerlet* >::iterator itSB = this->m_MapSoftBodiesByName.begin();
        itSB != this->m_MapSoftBodiesByName.end(); itSB++)
    {
        cSoftBodyVerlet* pCurrentSB = itSB->second;

        // Update normals
        pCurrentSB->UpdateNormals();

        // Find the corresponding mesh in the VAO
        sModelDrawInfo softBodyDrawMeshLocalCopy;
        this->m_pVAOManager->FindDrawInfoByModelName(pCurrentSB->matching_VAO_MeshName, softBodyDrawMeshLocalCopy);

        // Update the copy of the ModelDrawInfo to reflect the current soft body particle (vertex) info
        for (unsigned int vertexIndex = 0; vertexIndex != softBodyDrawMeshLocalCopy.numberOfVertices; vertexIndex++)
        {
            softBodyDrawMeshLocalCopy.pVertices[vertexIndex].x = pCurrentSB->vec_pParticles[vertexIndex]->position.x;
            softBodyDrawMeshLocalCopy.pVertices[vertexIndex].y = pCurrentSB->vec_pParticles[vertexIndex]->position.y;
            softBodyDrawMeshLocalCopy.pVertices[vertexIndex].z = pCurrentSB->vec_pParticles[vertexIndex]->position.z;

            // Also the normals...
            softBodyDrawMeshLocalCopy.pVertices[vertexIndex].nx = pCurrentSB->vec_pParticles[vertexIndex]->pModelVertex->nx;
            softBodyDrawMeshLocalCopy.pVertices[vertexIndex].ny = pCurrentSB->vec_pParticles[vertexIndex]->pModelVertex->ny;
            softBodyDrawMeshLocalCopy.pVertices[vertexIndex].nz = pCurrentSB->vec_pParticles[vertexIndex]->pModelVertex->nz;

            // Also anything else (colours? UVs?)

        }//for (unsigned int vertexIndex

        // Copy this back to the VAO (overwriting the current VAO information)
        this->m_pVAOManager->UpdateDynamicMesh(pCurrentSB->matching_VAO_MeshName, softBodyDrawMeshLocalCopy, shaderProgramID);

    }
    return;
}





// Moved to cPhysics_cSoftBody_Verlet_Threaded.cpp
// 
//    void cPhysics::updateSoftBodies(double deltaTime)
//    {
//        for (std::map< std::string, cSoftBodyVerlet* >::iterator itSB = this->m_MapSoftBodiesByName.begin();
//            itSB != this->m_MapSoftBodiesByName.end(); itSB++)
//        {
//            cSoftBodyVerlet* pCurrentSB = itSB->second;
//
//            // Like the Euler step, but we are using Verlet
//            // (i.e. calculate the next (or current) location based on gravity, etc.
//            pCurrentSB->VerletUpdate(deltaTime);
//
//            // Update for collision constraints
//            pCurrentSB->ApplyCollision(deltaTime);
//
//
//            pCurrentSB->SatisfyConstraints();
//
//            //        pCurrentSB->UpdateDrawInfo();
//        }
//        return;
//    }
