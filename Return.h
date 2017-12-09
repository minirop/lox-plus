//
// Created by minirop on 30/11/17.
//

#ifndef LOXPLUS_RETURN_H
#define LOXPLUS_RETURN_H


#include "Object.h"

class Return
{
public:
    explicit Return(Object value);

    Return(const Return &) = delete;
    Return(Return &&) = default;
    Return & operator=(const Return &) = delete;
    Return & operator=(Return &&) = default;

    Object value;
};


#endif //LOXPLUS_RETURN_H
