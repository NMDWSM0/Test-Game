#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H

#include "entity.h"
#include "sourcemodifierlist.h"

class Component
{
protected:
	Entity* const inst;

public:
	Component(Entity* const inst):
		inst(inst)
	{

	}

	virtual ~Component() {}
};

#endif // !COMPONENT_H
