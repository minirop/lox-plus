//
// Created by minirop on 02/12/17.
//

#ifndef LOXPLUS_LOXINSTANCE_H
#define LOXPLUS_LOXINSTANCE_H

#include "LoxClass.h"

class Token;

class LoxInstance : public CreatableType<LoxInstance>
{
public:
    explicit LoxInstance(LoxClass * klass);

    Object get(Token name);
    void set(Token name, Object value);

    LoxClass * getClass() const { return klass; }

private:
    LoxClass * klass;
    std::map<std::string, Object> fields;
};

#endif //LOXPLUS_LOXINSTANCE_H
