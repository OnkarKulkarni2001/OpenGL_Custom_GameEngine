#include "cCommandGroup.h"

cCommandGroup::cCommandGroup()
{
}

void cCommandGroup::addSerial(iCommand* pNewSerialCommand)
{
	this->m_vecSerialCommands.push_back(pNewSerialCommand);
	// Was this the 1st (and only) command added? 
	if (this->m_vecSerialCommands.size() == 1)
	{
		// Yes, so call start on this
		this->m_vecSerialCommands[0]->OnStart();
	}
	return;
}

void cCommandGroup::addParallel(iCommand* pNewParallelCommand)
{
	this->m_vecParallelCommands.push_back(pNewParallelCommand);
	return;
}


void cCommandGroup::OnStart(void)
{
	// TODO: 
	return;
}

void cCommandGroup::Update(double deltaTime)
{
	// If the parallel commands are NOT done, call update
	for (iCommand* pCurPC : this->m_vecParallelCommands)
	{
		if ( ! pCurPC->isFinished() )
		{
			pCurPC->Update(deltaTime);
		}
	}
	//
	// Always run the 1st command in the list of serial commands
	// When the command is done, remove it
	if ( ! this->m_vecSerialCommands.empty() )
	{
		// There is at least one command in the vector of serial commands. 
		// Is this one done?
		if ( ! this->m_vecSerialCommands[0]->isFinished() )
		{
			// Call update
			this->m_vecSerialCommands[0]->Update(deltaTime);
		}
		else
		{
			// Let the command know it's done.
			this->m_vecSerialCommands[0]->OnFinished();

			// Move to the next command
			// 
			// Which means deleting the 1st command
			// 
			// TODO: Do we actually delete these? 
			// (Something to think about because if we don't, there's a memory leak)
			delete this->m_vecSerialCommands[0];
			//
			this->m_vecSerialCommands.erase(this->m_vecSerialCommands.begin());
			//
			// If there is a "next command" call OnStart()
			if (!this->m_vecSerialCommands.empty())
			{
				this->m_vecSerialCommands[0]->OnStart();
			}
		}//if ( ...->isFinished() 
	}//if ( ! this->m_vecSerialCommands.empty()

	return;
}
bool cCommandGroup::isFinished(void)
{
	// All parallel commands are done when all the commands in vector reutrn isFinished
	for (iCommand* pCurPC : this->m_vecParallelCommands)
	{
		if ( ! pCurPC->isFinished() )
		{
			// One of these ISN'T done
			return false;
		}
	}

	// Serial: is the iterator pointing to the end of the vector?
	if ( ! this->m_vecSerialCommands.empty() )
	{
		// No, so there are still active serial commands
		return false;
	}

	// every command is done
	return true;
}

void cCommandGroup::OnFinished(void)
{
	// TODO: 
	return;
}