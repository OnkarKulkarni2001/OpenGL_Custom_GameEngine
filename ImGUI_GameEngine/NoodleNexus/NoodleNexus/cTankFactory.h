#pragma once

// ONLY include the interface
// NEVER EVER the specific derrived types
#include "iTank.h"
#include <string>
#include <vector>

class cTankFactory
{
public:
	// This is the one you call:
	static cTankFactory* get_pTankFactory(void);

	// This seems like a solution to avoid pointers, 
	//	but it doesn't really mask that there's really a pointer
//    ____              _ _         _         _   _     _            
//   |  _ \  ___  _ __ ( ) |_    __| | ___   | |_| |__ (_)___        
//   | | | |/ _ \| '_ \|/| __|  / _` |/ _ \  | __| '_ \| / __|       
//   | |_| | (_) | | | | | |_  | (_| | (_) | | |_| | | | \__ \_ _ _  
//   |____/ \___/|_| |_|  \__|  \__,_|\___/   \__|_| |_|_|___(_|_|_) 
//                                                                   
	static cTankFactory& getTankFactory(void);

	// Factory method
	// Always pass a known type, not an enum, etc.
	// Returns NULL if unknown type
	iTank* CreateATank(std::string tankType);

	// Returns false if unknown type
//	bool CreateATank(std::string tankType, iTank*& pNewTank);
	// Etc.

	void GetTankTypes(std::vector<std::string>& vecTankTypeList);

private:

	// Private so can't be called (no new, etc.)
	cTankFactory();
	// This is the one and only instance
	static cTankFactory* m_pTheInstance;

	static int shoeSize;// = 0;

};
