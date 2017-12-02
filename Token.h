//
// Created by minirop on 04/11/17.
//

#ifndef LOXPLUS_TOKEN_H
#define LOXPLUS_TOKEN_H


#include <string_view>
#include <variant>
#include "TokenType.h"
#include "Object.h"

class Token
{
public:
    template <typename T>
    Token(TokenType type, std::string lexeme, T literal, std::size_t line)
        : type { type }, lexeme { std::move(lexeme) }, literal { literal }, line { line }
    {
    }

    const TokenType type;
    const std::string lexeme;
    const Object literal;
    const std::size_t line;
};

#endif //LOXPLUS_TOKEN_H
