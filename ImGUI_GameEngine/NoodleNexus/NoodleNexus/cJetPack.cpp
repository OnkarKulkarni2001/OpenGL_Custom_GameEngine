#include "cJetPack.h"
#include <iostream>

void cJetPack::Fly(glm::vec3 direction, float speed)
{
	std::cout << "Jet back enabled!" << std::endl;

	return;
}

// From the iMessaging interface
// // Sent to the recieving object
bool cJetPack::Receive_Message(sNVPair theMesssage)
{
	// TODO: insert amazing code

	return true;
}

// From the iMessaging interface
// // Sent to the mediator to pass along
bool cJetPack::Send_Message(sNVPair theMesssage)
{
	// TODO: insert amazing code

	return true;
}