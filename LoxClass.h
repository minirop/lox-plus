//
// Created by minirop on 02/12/17.
//

#ifndef LOXPLUS_LOXCLASS_H
#define LOXPLUS_LOXCLASS_H

#include <string>
#include "LoxCallable.h"

class LoxClass : public LoxCallable
{
public:
    LoxClass(std::string name, std::map<std::string, LoxFunction*> methods);
    Object call(Interpreter & interpreter, std::vector<Object> arguments) override;
    int arity() const override;

    LoxFunction* findMethod(LoxInstance* instance, std::string name);

    std::string getName() const override { return name; }

private:
    std::string name;
    std::map<std::string, LoxFunction*> methods;
};


#endif //LOXPLUS_LOXCLASS_H
