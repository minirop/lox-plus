//
// Created by minirop on 28/11/17.
//

#include <iostream>
#include "Environment.h"
#include "RuntimeError.h"

Environment::Environment(Environment * enclosing)
    : enclosing(enclosing)
{
}

void Environment::define(std::string name, Object value)
{
    values.emplace(name, value);
}

void Environment::assign(Token name, Object value)
{
    if (values.count(name.lexeme))
    {
        values[name.lexeme] = value;
        return;
    }

    if (enclosing != nullptr)
    {
        enclosing->assign(name, value);
        return;
    }

    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

Object Environment::get(Token name)
{
    if (values.count(name.lexeme))
    {
        return values[name.lexeme];
    }

    if (enclosing != nullptr) return enclosing->get(name);

    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

Object Environment::getAt(unsigned long distance, const std::string name)
{
    return ancestor(distance)->values[name];
}

Environment * Environment::ancestor(unsigned long distance)
{
    auto environment = this;
    for (unsigned long i = 0; i < distance; i++)
    {
        environment = environment->enclosing;
    }
    if (environment == nullptr)
    {
        std::cout << "BUUUUUUUUUUUUUUUUUUG: " << distance << "\n";
    }

    return environment;
}

void Environment::assignAt(unsigned long distance, Token name, Object value)
{
    ancestor(distance)->values[name.lexeme] = value;
}
