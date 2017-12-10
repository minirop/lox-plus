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

    Token() = delete;
    Token(const Token &) = default;
    Token(Token &&) = default;

    Token & operator=(const Token &) = default;
    Token & operator=(Token &&) = default;

    ~Token() = default;

    TokenType type;
    std::string lexeme;
    Object literal;
    std::size_t line;
};

#endif //LOXPLUS_TOKEN_H
