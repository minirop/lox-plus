//
// Created by minirop on 02/12/17.
//

#include <iostream>
#include "LoxInstance.h"
#include "RuntimeError.h"
#include "LoxFunction.h"

LoxInstance::LoxInstance(LoxClass * klass)
    : klass { klass }
{
}

Object LoxInstance::get(Token name)
{
    if (fields.count(name.lexeme))
    {
        return fields[name.lexeme];
    }

    auto method = klass->findMethod(this, name.lexeme);
    if (method != nullptr)
    {
        return method;
    }

    throw RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
}

void LoxInstance::set(Token name, Object value)
{
    fields[name.lexeme] = value;
}
