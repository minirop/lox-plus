//
// Created by minirop on 04/11/17.
//

#ifndef LOXPLUS_SCANNER_H
#define LOXPLUS_SCANNER_H


#include <string_view>
#include <vector>
#include <array>
#include <map>
#include "Token.h"
#include <iostream>

class Scanner
{
public:
    explicit Scanner(std::string_view source);

    Scanner(const Scanner &) = delete;
    Scanner(Scanner &&) = delete;
    Scanner & operator=(const Scanner &) = delete;
    Scanner & operator=(Scanner &&) = delete;
    ~Scanner() = default;

    std::vector<Token> scanTokens();
    bool isAtEnd();

private:
    void scanToken();
    char advance();
    void addToken(TokenType type);
    bool match(char expected);
    char peek();
    void string();
    bool isDigit(char c);
    void number();
    char peekNext();
    bool isAlpha(char c);
    void identifier();
    bool isAlphaNumeric(char peek);

    template <typename T>
    void addToken(TokenType type, T literal);

    std::string_view source;
    std::vector<Token> tokens;
    std::size_t start = 0;
    std::size_t current = 0;
    std::size_t line = 1;

    const std::map<std::string_view, TokenType> keywords = {
        { "and",    TokenType::AND    },
        { "class",  TokenType::CLASS  },
        { "else",   TokenType::ELSE   },
        { "false",  TokenType::FALSE  },
        { "for",    TokenType::FOR    },
        { "fun",    TokenType::FUN    },
        { "if",     TokenType::IF     },
        { "nil",    TokenType::NIL    },
        { "or",     TokenType::OR     },
        { "print",  TokenType::PRINT  },
        { "return", TokenType::RETURN },
        { "super",  TokenType::SUPER  },
        { "this",   TokenType::THIS   },
        { "true",   TokenType::TRUE   },
        { "var",    TokenType::VAR    },
        { "while",  TokenType::WHILE  }
    };
};

template <typename T>
void Scanner::addToken(TokenType type, T literal)
{
    auto text = std::string { source.substr(start, current-start) };
    tokens.emplace_back(type, text, literal, line);
}

#endif //LOXPLUS_SCANNER_H
