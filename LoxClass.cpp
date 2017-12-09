//
// Created by minirop on 02/12/17.
//

#include <iostream>
#include "LoxClass.h"
#include "LoxInstance.h"
#include "LoxFunction.h"

LoxClass::LoxClass(std::string name, std::map<std::string, std::shared_ptr<LoxFunction>> && methods)
    : name { std::move(name) }, methods { std::move(methods) }
{
}

Object LoxClass::call(Interpreter & interpreter, std::vector<Object> arguments)
{
    auto instance = std::make_shared<LoxInstance>(shared_from_this());

    if (methods.count("init"))
    {
        auto initializer = methods["init"].get();
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

std::shared_ptr<LoxFunction> LoxClass::findMethod(std::shared_ptr<LoxInstance> instance, std::string name)
{
    if (methods.count(name))
    {
        return methods[name]->bind(std::move(instance));
    }

    return nullptr;
}
