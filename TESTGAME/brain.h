#pragma once
#ifndef BRAIN_H
#define BRAIN_H

#include "monster.h"
#include "externfuncs.h"

class Brain
{
protected:
	Monster* const inst;

	bool enable = true;

public:
	Brain(Monster* const inst) :
		inst(inst)
	{

	}

	virtual void UpdateActions()
	{

	}

	virtual void Stop()
	{
		enable = false;
	}

	virtual void Start()
	{
		enable = true;
	}
};

#endif // !BRAIN_H
