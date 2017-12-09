//
// Created by minirop on 30/11/17.
//

#ifndef LOXPLUS_LOXCALLABLE_H
#define LOXPLUS_LOXCALLABLE_H

#include <vector>
#include "Object.h"
#include "Interpreter.h"

class LoxCallable
{
public:
    LoxCallable(const LoxClass &) = delete;
    LoxCallable(LoxClass &&) = delete;
    LoxCallable & operator=(const LoxClass &) = delete;
    LoxCallable & operator=(LoxClass &&) = delete;

    virtual ~LoxCallable() = default;
    virtual Object call(Interpreter & interpreter, std::vector<Object> arguments) = 0;
    virtual int arity() const = 0;

    virtual std::string getName() const = 0;

protected:
    LoxCallable() = default;
};

#endif //LOXPLUS_LOXCALLABLE_H
