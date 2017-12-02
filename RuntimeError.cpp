//
// Created by minirop on 30/11/17.
//

#include "RuntimeError.h"

RuntimeError::RuntimeError(Token token, std::string message)
    : std::runtime_error(message), token { token }
{

}
