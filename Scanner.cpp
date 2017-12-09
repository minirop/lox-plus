//
// Created by minirop on 04/11/17.
//

#include <cctype>
#include <string>
#include <iostream>
#include "Scanner.h"
#include "Lox-plus.h"

Scanner::Scanner(std::string_view source)
    : source { source }
{

}

std::vector<Token> Scanner::scanTokens()
{
    while (!isAtEnd())
    {
        // We are at the beginning of the next lexeme.
        start = current;
        scanToken();
    }

    tokens.emplace_back(TokenType::EOF, "", nullptr, line);
    return tokens;
}

bool Scanner::isAtEnd()
{
    return current >= source.size();
}

void Scanner::scanToken()
{
    char c = advance();
    switch (c)
    {
        case '(': addToken(TokenType::LEFT_PAREN); break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '{': addToken(TokenType::LEFT_BRACE); break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case ',': addToken(TokenType::COMMA); break;
        case '.': addToken(TokenType::DOT); break;
        case '-': addToken(TokenType::MINUS); break;
        case '+': addToken(TokenType::PLUS); break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case '*': addToken(TokenType::STAR); break;

        case '!': addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
        case '=': addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
        case '<': addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
        case '>': addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;

        case '/':
            if (match('/'))
            {
                // A comment goes until the end of the line.
                while (peek() != '\n' && !isAtEnd()) advance();
            }
            else
            {
                addToken(TokenType::SLASH);
            }
            break;

        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace.
            break;

        case '\n':
            line++;
            break;

        case '"': string(); break;

        default:
            if (isDigit(c))
            {
                number();
            }
            else if (isAlpha(c))
            {
                identifier();
            }
            else
            {
                LoxPlus::error(line, "Unexpected character.");
            }
            break;
    }
}

char Scanner::advance()
{
    current++;
    return source.at(current - 1);
}

void Scanner::addToken(TokenType type)
{
    addToken(type, nullptr);
}

bool Scanner::match(char expected)
{
    if (isAtEnd()) return false;
    if (source.at(current) != expected) return false;

    current++;
    return true;
}

char Scanner::peek()
{
    if (isAtEnd()) return '\0';
    return source.at(current);
}

void Scanner::string()
{
    while (peek() != '"' && !isAtEnd())
    {
        if (peek() == '\n') line++;
        advance();
    }

    // Unterminated string.
    if (isAtEnd())
    {
        LoxPlus::error(line, "Unterminated string.");
        return;
    }

    // The closing ".
    advance();

    // Trim the surrounding quotes.
    std::string_view value = source.substr(start + 1, (current - start) - 2);
    addToken(TokenType::STRING, std::string { value });
}

bool Scanner::isDigit(char c)
{
    return std::isdigit(c);
}

void Scanner::number()
{
    while (isDigit(peek())) advance();

    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext()))
    {
        // Consume the "."
        advance();

        while (isDigit(peek())) advance();
    }

    auto value_str = source.substr(start, current - start);
    double d = std::stod(std::string(value_str));
    addToken(TokenType::NUMBER, d);
}

char Scanner::peekNext()
{
    if (current + 1 >= source.size()) return '\0';
    return source.at(current + 1);
}

bool Scanner::isAlpha(char c)
{
    return std::isalpha(c) || c == '_';
}

void Scanner::identifier()
{
    while (isAlphaNumeric(peek())) advance();

    const std::string_view text = source.substr(start, current - start);

    TokenType type = TokenType::IDENTIFIER;
    if (keywords.count(text))
    {
        type = keywords.at(text);
    }

    addToken(type);
}

bool Scanner::isAlphaNumeric(char peek)
{
    return isAlpha(peek) || isDigit(peek);
}
