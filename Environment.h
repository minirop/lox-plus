//
// Created by minirop on 28/11/17.
//

#ifndef LOXPLUS_ENVIRONMENT_H
#define LOXPLUS_ENVIRONMENT_H

#include <map>
#include <string>
#include "Object.h"
#include "Token.h"

class Environment
{
public:
    Environment() = default;
    explicit Environment(Environment * enclosing);

    void define(std::string name, Object value);
    void assign(Token name, Object value);
    Object get(Token name);

    Object getAt(unsigned long distance, std::string name);

    void assignAt(unsigned long distance, Token name, Object value);

private:
    std::map<std::string, Object> values;
    Environment * enclosing = nullptr;

    Environment * ancestor(unsigned long distance);
};


#endif //LOXPLUS_ENVIRONMENT_H
