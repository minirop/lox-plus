//
// Created by minirop on 02/12/17.
//

#include <iostream>
#include "LoxClass.h"
#include "LoxInstance.h"
#include "LoxFunction.h"

LoxClass::LoxClass(std::string name, std::map<std::string, LoxFunction*> methods)
    : name { name }, methods { methods }
{
}

Object LoxClass::call(Interpreter & interpreter, std::vector<Object> arguments)
{
    auto instance = new LoxInstance(this);

    if (methods.count("init"))
    {
        auto initializer = methods["init"];
        initializer->bind(instance)->call(interpreter, arguments);
    }

    return instance;
}

int LoxClass::arity() const
{
    if (methods.count("init"))
    {
        return methods.at("init")->arity();
    }
    return 0;
}

LoxFunction * LoxClass::findMethod(LoxInstance * instance, std::string name)
{
    if (methods.count(name))
    {
        return methods[name]->bind(instance);
    }

    return nullptr;
}
