#pragma once
// All commands have this interface

// This would benefit form being part of an Abstract Factory pattern.
// Because all the commands are "a type of iCommand"

class iCommand
{
public:
	// Pure virtual objects require a virtual destrctor (because C++)
	virtual ~iCommand() {};
	virtual void OnStart(void) = 0;
	virtual void Update(double deltaTime) = 0;
	virtual bool isFinished(void) = 0;
	virtual void OnFinished(void) = 0;
};
