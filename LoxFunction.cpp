//
// Created by minirop on 30/11/17.
//

#include "LoxFunction.h"
#include "Return.h"
#include "LoxInstance.h"

LoxFunction::LoxFunction(FunctionStmt* declaration, Environment* closure, bool isInitializer)
    : declaration { declaration }, closure { closure }, isInitializer { isInitializer }
{

}

Object LoxFunction::call(Interpreter & interpreter, std::vector<Object> arguments)
{
    auto environment = Environment::create(closure);
    int i = 0;
    for (auto & parameter : declaration->parameters)
    {
        environment->define(parameter.lexeme, arguments[i++]);
    }

    try
    {
        interpreter.executeBlock(declaration->body, environment);
    }
    catch (const Return & returnValue)
    {
        return returnValue.value;
    }

    if (isInitializer) return closure->getAt(0, "this");
    return Object();
}

int LoxFunction::arity() const
{
    // safe cast since max number of arguments is 8.
    return static_cast<int>(declaration->parameters.size());
}

LoxFunction* LoxFunction::bind(LoxInstance* instance)
{
    auto environment = Environment::create(closure);
    environment->define("this", instance);
    return LoxFunction::create(declaration, environment, isInitializer);
}
