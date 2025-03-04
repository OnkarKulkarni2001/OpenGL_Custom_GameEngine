#pragma once

// This builder also "knows" about the various 
//	weapons, etc. that the tank factory doesn't

#include <string>
#include <vector>
#include "iTank.h"
//#include "cTankFactory.h"

class cTankBuilder
{
public:
	cTankBuilder();

	// A "factory method"// This is the one you call:
	iTank* CreateATank(std::string tankType);

	void GetTankTypes(std::vector<std::string>& vecTankTypeList);


//	// or something a little more fancy
//	iTank* CreateATenk(std::string tankType,
//	                   std::vector< std::string > vecAccessories);
private:
//	cTankFactory* m_pTankFactory;
};
