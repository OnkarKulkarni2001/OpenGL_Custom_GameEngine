// cPhysics_cSoftBody_Verlet_Threaded.cpp

#include "cPhysics.h"

#include <Windows.h>
// or #include <process.h>   if you are using _beginthreadex() instead of CreateThread(), etc.


struct sVerletUpdateData
{
    cPhysics* pPhysicsObject;   // For the "this" pointer
    double deltaTime;
};

bool cs_isInitialized = false;
CRITICAL_SECTION cs_verletThread;
// The thread will set this to true when it's finished
bool verletThreadDone = true;

// https://learn.microsoft.com/en-us/previous-versions/windows/desktop/legacy/ms686736(v=vs.85)
// 
// our thread function, with this specific signature
DWORD WINAPI threadFunction_VerletUpdate(LPVOID lpParameter)
{
    // Cast it back to the type we know it is...
    sVerletUpdateData* pVerletData = (sVerletUpdateData*)lpParameter;

    for (std::map< std::string, cSoftBodyVerlet* >::iterator itSB 
            = pVerletData->pPhysicsObject->m_MapSoftBodiesByName.begin();
        itSB != pVerletData->pPhysicsObject->m_MapSoftBodiesByName.end(); itSB++)
    {
        cSoftBodyVerlet* pCurrentSB = itSB->second;

        // Like the Euler step, but we are using Verlet
        // (i.e. calculate the next (or current) location based on gravity, etc.
        pCurrentSB->VerletUpdate(pVerletData->deltaTime);

        // Update for collision constraints
        pCurrentSB->ApplyCollision(pVerletData->deltaTime);


        pCurrentSB->SatisfyConstraints();

        //        pCurrentSB->UpdateDrawInfo();
    }

    // Let everyone know I'm done
//    EnterCriticalSection(&cs_verletThread);
    verletThreadDone = true;
//    LeaveCriticalSection(&cs_verletThread);

    // Note that this is deleted here, because the thread is now "resposible" for that data
    delete pVerletData;

    return 0;
}



void cPhysics::updateSoftBodies(double deltaTime)
{
    // Is the critical section set up
    if ( ! cs_isInitialized )
    {
        InitializeCriticalSection(&cs_verletThread);
        cs_isInitialized = true;
    }


    // Set up the information for the thread

    // Get the address of the this object and save it into a pointer variable
//    sVerletUpdateData VerletUpdateInfo;
// 
//    VerletUpdateInfo.pPhysicsObject = this;
//    VerletUpdateInfo.deltaTime = deltaTime;
// 
//    sVerletUpdateData* pVUI = &VerletUpdateInfo;
//    void* pDataForThread = (void*)pVUI;
//    threadFunction_VerletUpdate(pDataForThread);

    // Usually you'll see this...  
//    threadFunction_VerletUpdate( (void*)&VerletUpdateInfo );

    // Has the last thread finished?
//    EnterCriticalSection(&cs_verletThread);
    if (verletThreadDone)
    {
        // Create another thread

        verletThreadDone = false;
 //       LeaveCriticalSection(&cs_verletThread);

        sVerletUpdateData* pVerletUpdateInfo = new sVerletUpdateData();

        pVerletUpdateInfo->pPhysicsObject = this;
        pVerletUpdateInfo->deltaTime = deltaTime;

        // https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createthread
        HANDLE hVerletUpdateThread
            = CreateThread(
                NULL,                           // LPSECURITY_ATTRIBUTES lpThreadAttributes
                0,                              // SIZE_T dwStackSize,
                threadFunction_VerletUpdate,    // LPTHREAD_START_ROUTINE lpStartAddress,
                (void*)pVerletUpdateInfo,       //__drv_aliasesMem LPVOID lpParameter,
                0,                              // DWORD dwCreationFlags,
                NULL);                          // LPDWORD lpThreadId
    }

//    delete pVerletUpdateInfo;

//   for (std::map< std::string, cSoftBodyVerlet* >::iterator itSB = this->m_MapSoftBodiesByName.begin();
//       itSB != this->m_MapSoftBodiesByName.end(); itSB++)
//   {
//       cSoftBodyVerlet* pCurrentSB = itSB->second;
//
//       // Like the Euler step, but we are using Verlet
//       // (i.e. calculate the next (or current) location based on gravity, etc.
//       pCurrentSB->VerletUpdate(deltaTime);
//
//       // Update for collision constraints
//       pCurrentSB->ApplyCollision(deltaTime);
//
//
//       pCurrentSB->SatisfyConstraints();
//
//       //        pCurrentSB->UpdateDrawInfo();
//   }
    return;
}
