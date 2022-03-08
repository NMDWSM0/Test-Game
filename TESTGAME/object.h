#pragma once
#ifndef OBJECT_H
#define OBJECT_H
#include "shader.h"

class Object
{
protected:
    virtual void Draw(Shader& shader) = 0;

public:
    virtual ~Object() {};
};

#endif