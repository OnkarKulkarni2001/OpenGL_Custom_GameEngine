#ifndef _iMessaging_HG_
#define _iMessaging_HG_

// This is sort of like the OG "original" Quake did
// had a string + vec3, basically

#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <string>
#include <vector>



class iMessaging;
struct sNVPair
{
	std::string theCommand;
	std::string sDetails;
	std::vector< glm::vec4 > vecDetails;
	iMessaging* pTheCaller;
};

// Sword sends to monster
// "AttackMonster"
// vecDetials[0] = vec3 location of sword
// vecDetails[1] = float amount of damage

// Is part of the "Mediator" pattern
class iMessaging
{
public:
	// Is pure virtual, so add a virual destructor
	virtual ~iMessaging() {};
	// Sent to the recieving object
	virtual bool Receive_Message(sNVPair theMesssage) = 0;
	// Sent to the mediator to pass along
	virtual bool Send_Message(sNVPair theMesssage) = 0;
};

#endif // _iMediator_HG_
