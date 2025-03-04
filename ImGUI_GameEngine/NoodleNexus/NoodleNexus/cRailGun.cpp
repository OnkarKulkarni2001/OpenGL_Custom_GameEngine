#include "cRailGun.h"

#include <iostream>

void cRailGun::Shoot(iTank* pTarget)
{
	std::cout << "Rail gun goes: pew! Zapp!" << std::endl;
	return;
}


// From the iMessaging interface
// // Sent to the recieving object
bool cRailGun::Receive_Message(sNVPair theMesssage)
{
	// TODO: insert amazing code

	return true;
}

// From the iMessaging interface
// // Sent to the mediator to pass along
bool cRailGun::Send_Message(sNVPair theMesssage)
{
	// TODO: insert amazing code

	return true;
}