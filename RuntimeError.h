//
// Created by minirop on 30/11/17.
//

#ifndef LOXPLUS_RUNTIMEERROR_H
#define LOXPLUS_RUNTIMEERROR_H


#include <stdexcept>
#include "Token.h"

class RuntimeError : public std::runtime_error
{
public:
    RuntimeError(Token token, std::string message);

    const Token token;
};


#endif //LOXPLUS_RUNTIMEERROR_H
