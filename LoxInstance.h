//
// Created by minirop on 02/12/17.
//

#ifndef LOXPLUS_LOXINSTANCE_H
#define LOXPLUS_LOXINSTANCE_H

#include "LoxClass.h"

class Token;

class LoxInstance : public std::enable_shared_from_this<LoxInstance>
{
public:
    explicit LoxInstance(std::shared_ptr<LoxClass> klass);

    Object get(Token name);
    void set(Token name, Object value);

    std::shared_ptr<LoxClass> getClass() const { return klass; }

private:
    std::shared_ptr<LoxClass> klass;
    std::map<std::string, Object> fields;
};

#endif //LOXPLUS_LOXINSTANCE_H
