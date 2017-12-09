//
// Created by minirop on 30/11/17.
//

#include "LoxFunction.h"
#include "Return.h"
#include "LoxInstance.h"

LoxFunction::LoxFunction(FunctionStmt & declaration, std::shared_ptr<Environment> closure, bool isInitializer)
    : declaration { declaration }, closure { std::move(closure) }, isInitializer { isInitializer }
{

}

Object LoxFunction::call(Interpreter & interpreter, std::vector<Object> arguments)
{
    auto environment = std::make_shared<Environment>(closure.get());
    int i = 0;
    for (auto & parameter : declaration.parameters)
    {
        environment->define(parameter.lexeme, arguments[i++]);
    }

    try
    {
        interpreter.executeBlock(declaration.body, environment);
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
    return static_cast<int>(declaration.parameters.size());
}

std::unique_ptr<LoxFunction> LoxFunction::bind(std::shared_ptr<LoxInstance> instance)
{
    auto environment = std::make_shared<Environment>(closure.get());
    environment->define("this", std::move(instance));
    return std::make_unique<LoxFunction>(declaration, std::move(environment), isInitializer);
}
