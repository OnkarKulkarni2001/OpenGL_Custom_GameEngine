#ifndef _cArena_HG_
#define _cArena_HG_

// This arena is a meditor type

#include "iMessaging.h"
// Arena also knows about the tanks, specifically
#include "iTank.h"

class cArena : 
	public iMessaging
{
public:

	// From the iMessaging interface
	virtual bool Receive_Message(sNVPair theMesssage);// Sent to the recieving object	
	virtual bool Send_Message(sNVPair theMesssage);// Sent to the mediator to pass along

	// Adds items to the mediator
	// (i.e. the mediator can call these things)
	void AddEntity(iMessaging* pTheThing);
	//
	void AddTank(iTank* pNewTank);

private:
	std::vector< iMessaging* > m_vec_pThings;
	// And the tanks (which are also "things"
	std::vector< iTank* > m_vec_pTanks;
};

#endif  // _cArena_HG_
