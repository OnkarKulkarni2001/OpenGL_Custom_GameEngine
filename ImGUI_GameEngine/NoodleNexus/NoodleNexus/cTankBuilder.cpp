#include "cTankBuilder.h"


#include "cTankFactory.h"
// Include the specific tanks
#include "cRailGun.h"
#include "cJetPack.h"
#include "cSheild.h"
#include "cTank.h"
#include "cSuperTank.h"
#include "cHoverTank.h"

cTankBuilder::cTankBuilder()
{
}

// A "factory method"// This is the one you call:
iTank* cTankBuilder::CreateATank(std::string tankType)
{
	if (tankType == "Regular Tank with Shield")
	{
		cTank* pRT = (cTank*)cTankFactory::get_pTankFactory()->CreateATank("Regular Tank");
		// Add a rail gun
		pRT->pTheShield = new cSheild();
		// If it has a sheild, then it's health is higher
		pRT->health = 150.0f;

		return pRT;
	}
	if (tankType == "Super Tank!")
	{
		cSuperTank* pST = (cSuperTank*)cTankFactory::get_pTankFactory()->CreateATank("Super Tank");
		// Add a rail gun
		pST->pTheShield[0] = new cSheild();
		pST->pTheShield[1] = new cSheild();
		// Add rail gun
		pST->pTheRailGun = new cRailGun();

		return pST;

	}
	if (tankType == "Hover Tank")
	{
		cHoverTank* pHT = (cHoverTank*)cTankFactory::get_pTankFactory()->CreateATank("Hover Tank");
		// Add a jet pack
		pHT->pTheJetPack = new cJetPack();
		// Add a shield
		pHT->pShield = new cSheild();


		return pHT;

	}

	// Unknown type
	return NULL;	// or 0 or nullptr
}

void cTankBuilder::GetTankTypes(std::vector<std::string>& vecTankTypeList)
{
	// I know how to make these...
	vecTankTypeList.push_back("Regular Tank with Shield");
	vecTankTypeList.push_back("Super Tank");
	vecTankTypeList.push_back("Hover Tank");
	return;
}