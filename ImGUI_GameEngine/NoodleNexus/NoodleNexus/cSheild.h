#pragma once

#include "iMessaging.h"

class cSheild :
	public iMessaging
{
public:
	cSheild();
	void Raise(void);
	void Lower(void);
	bool bIsUp(void);

	// ********************************
// From the iMessaging interface
//		This method is called BY the mediator
	virtual bool Receive_Message(sNVPair theMesssage);
	//		Normally this is what we sent to the mediator
	virtual bool Send_Message(sNVPair theMesssage);
	// ********************************


private:
	bool m_bIsRaised;
};