//
// Created by minirop on 02/12/17.
//

#include <iostream>
#include "LoxInstance.h"
#include "RuntimeError.h"
#include "LoxFunction.h"

LoxInstance::LoxInstance(std::shared_ptr<LoxClass> klass)
    : klass { std::move(klass) }
{
}

Object LoxInstance::get(Token name)
{
    if (fields.count(name.lexeme))
    {
        return fields[name.lexeme];
    }

    auto method = klass->findMethod(shared_from_this(), name.lexeme);
    if (method)
    {
        return method;
    }

    throw RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
}

void LoxInstance::set(Token name, Object value)
{
    fields[name.lexeme] = std::move(value);
}
