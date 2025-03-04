#include "sharedThings.h"
#include <sstream>
cBasicFlyCamera* g_pFlyCamera = NULL;

cLightManager* g_pLightManager = NULL;
unsigned int g_selectedLightIndex = 0;
bool g_bShowDebugSpheres = false;

// I'm putting this here
cLuaBrain* g_pMyLuaMasterBrain = NULL;

glm::vec3 g_rgb_from_HTML(unsigned int red, unsigned int green, unsigned int blue)
{
	return glm::vec3((float)red / 255.0f, (float)green / 255.0f, (float)blue / 255.0f);
}

// HACK: Show "targeting LASER"
bool g_bShowLASERBeam = true;

std::string g_floatToString(float theFloat)
{
	std::stringstream ssFloat;
	ssFloat << theFloat;
	return ssFloat.str();
}