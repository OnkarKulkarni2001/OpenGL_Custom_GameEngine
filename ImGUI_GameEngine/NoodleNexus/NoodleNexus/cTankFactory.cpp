#include "cTankFactory.h"

// HERE (in the cpp or implementation) is where the #includes
// for the specific types go

#include "cTank.h"
#include "cSuperTank.h"
#include "cHoverTank.h"
#include <iostream>

//const std::string TANK_FACTORY_REGULAR_TANK = "Regular Tank";
//const std::string TANK_FACTORY_SUPER_TANK = "Super Tank";

cTankFactory::cTankFactory()
{
	//this->shoeSize = 0;
	std::cout << "A tank factory has been created" << std::endl;
}

// This is how static variables need to be initialized
//static
int cTankFactory::shoeSize = 0;

/*static*/ 
cTankFactory* cTankFactory::m_pTheInstance = NULL;

/*static*/ 
cTankFactory* cTankFactory::get_pTankFactory(void)
{
	// Created one, yet?
	if (!cTankFactory::m_pTheInstance)	// Or compare to NULL
	{
		// Create it
		cTankFactory::m_pTheInstance = new cTankFactory();
	}
	// Return the one instance
	return cTankFactory::m_pTheInstance;
}

/*static*/ 
cTankFactory& cTankFactory::getTankFactory(void)
{
	// Created one, yet?
	if (!cTankFactory::m_pTheInstance)	// Or compare to NULL
	{
		// Create it
		cTankFactory::m_pTheInstance = new cTankFactory();
	}
	
	// Return a de-referenced instance
	return *cTankFactory::m_pTheInstance;
}




// Returns NULL if unknown type
iTank* cTankFactory::CreateATank(std::string tankType)
{
	if (tankType == "Regular Tank")
	{
		return new cTank();
	}
	if (tankType == "Super Tank")
	{
		return new cSuperTank();
	}
	if (tankType == "Hover Tank")
	{
		return new cHoverTank();
	}

	// Unknown type
	return NULL;	// or 0 or nullptr
}

void cTankFactory::GetTankTypes(std::vector<std::string>& vecTankTypeList)
{
	// I know how to make these...
	vecTankTypeList.push_back("Regular Tank");
	vecTankTypeList.push_back("Super Tank");
	vecTankTypeList.push_back("Hover Tank");
	// 
	vecTankTypeList.push_back("LASER Tank");
	return;
}
	
