//
// Created by minirop on 04/11/17.
//

#include "Lox-plus.h"
#include "Scanner.h"
#include "ast.h"
#include "Parser.h"
#include "Interpreter.h"
#include "Resolver.h"
#include <fstream>

using namespace std::string_literals;

void LoxPlus::runPrompt()
{
    runFile("test.lox");
}

int LoxPlus::runFile(const char* name)
{
    std::ifstream file { name, std::ios::binary | std::ios::ate };

    if (file.is_open())
    {
        auto size = file.tellg();
        std::string str(static_cast<unsigned long>(size), '\0');
        file.seekg(0);
        if(file.read(&str[0], size))
        {
            run(str);
        }
        file.close();
    }

    if (hadError) return 65;
    if (hadRuntimeError) return 70;

    return 0;
}

void LoxPlus::run(std::string_view source)
{
    Scanner scanner(source);
    auto tokens = scanner.scanTokens();

    Parser parser(tokens);
    auto statements = parser.parse();

    // Stop if there was a syntax error.
    if (hadError) return;

    Interpreter interpreter;

    Resolver resolver(interpreter);
    resolver.resolve(statements);

    // Stop if there was a resolution error.
    if (hadError) return;

    interpreter.interpret(statements);
}

void LoxPlus::error(std::size_t line, std::string_view message)
{
    report(line, "", message);
}

void LoxPlus::report(std::size_t line, std::string_view where, std::string_view message)
{
    std::cout << "[line " << line << "] Error" << where << ": " << message << '\n';
    hadError = true;
}

void LoxPlus::error(const Token & token, std::string_view message)
{
    if (token.type == TokenType::EOF)
    {
        report(token.line, " at end", message);
    }
    else
    {
        report(token.line, " at '"s + std::string { token.lexeme } + "'"s, message);
    }
}

void LoxPlus::runtimeError(const RuntimeError & error)
{
    std::cout << "[line " << error.token.line << "] " << error.what() << '\n';
    hadRuntimeError = true;
}
