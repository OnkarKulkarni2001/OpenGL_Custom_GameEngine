#pragma once

#include "iCommand.h"
#include <vector>

class cCommandGroup : public iCommand
{
public:
	cCommandGroup();
	void addSerial(iCommand* pNewSerialCommand);
	void addParallel(iCommand* pNewSepNewParallelCommandrialCommand);

	// From the iCommand interface
	virtual void OnStart(void);
	virtual void Update(double deltaTime);
	virtual bool isFinished(void);
	virtual void OnFinished(void);

private:
	std::vector< iCommand* > m_vecSerialCommands;
	std::vector< iCommand* > m_vecParallelCommands;
	
	// In order to have a series of parallel and serial commands in sequence
	// Like in the "Concurrency" image here:
	// https://docs.wpilib.org/en/2021/docs/software/old-commandbased/basics/what-is-command-based.html
	// 
	// You could have the command groups hold other command groups
	std::vector< cCommandGroup* > m_vecSerialCommandGroups;
};