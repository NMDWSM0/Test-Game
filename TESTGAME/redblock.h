#pragma once
#ifndef REDBLOCK_H
#define REDBLOCK_H

#include "entity.h"
class RedBlock : public Entity
{
public:
	RedBlock():
		Entity("assets/prefabs/redblock.txt", "assets/images/redblock.png")
	{

	}
};

#endif