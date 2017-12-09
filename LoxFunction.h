//
// Created by minirop on 30/11/17.
//

#ifndef LOXPLUS_LOXFUNCTION_H
#define LOXPLUS_LOXFUNCTION_H

#include "LoxCallable.h"

class LoxInstance;

class LoxFunction : public LoxCallable
{
public:
    LoxFunction(FunctionStmt & declaration, std::shared_ptr<Environment> closure, bool isInitializer);
    Object call(Interpreter & interpreter, std::vector<Object> arguments) override;
    int arity() const override;

    LoxFunction(const LoxFunction &) = delete;
    LoxFunction(LoxFunction &&) = default;

    LoxFunction & operator=(const LoxFunction &) = delete;
    LoxFunction & operator=(LoxFunction &&) = default;
    ~LoxFunction() = default;

    std::unique_ptr<LoxFunction> bind(std::shared_ptr<LoxInstance> instance);

    std::string getName() const override { return "<fun>"; }

private:
    FunctionStmt & declaration;
    std::shared_ptr<Environment> closure;
    bool isInitializer;
};


#endif //LOXPLUS_LOXFUNCTION_H
