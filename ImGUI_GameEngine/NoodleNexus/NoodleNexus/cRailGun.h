#pragma once

#include "iTank.h"
#include "iMessaging.h"

class cRailGun :
	public iMessaging
{
public:
	void Shoot(iTank* pTarget);

	// ********************************
	// From the iMessaging interface
	//		This method is called BY the mediator
	virtual bool Receive_Message(sNVPair theMesssage);
	//		Normally this is what we sent to the mediator
	virtual bool Send_Message(sNVPair theMesssage);
	// ********************************


};