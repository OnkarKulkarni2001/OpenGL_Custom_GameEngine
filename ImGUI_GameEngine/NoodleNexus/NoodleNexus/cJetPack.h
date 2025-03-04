#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include "iMessaging.h"

class cJetPack :
	public iMessaging
{
public:

	void Fly(glm::vec3 direction, float speed);

	// ********************************
	// From the iMessaging interface
	//		This method is called BY the mediator
	virtual bool Receive_Message(sNVPair theMesssage);
	//		Normally this is what we sent to the mediator
	virtual bool Send_Message(sNVPair theMesssage);
	// ********************************


};