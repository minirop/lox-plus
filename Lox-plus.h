//
// Created by minirop on 04/11/17.
//

#ifndef LOXPLUS_LOXPLUS_H
#define LOXPLUS_LOXPLUS_H

#include <string_view>
#include "RuntimeError.h"

class Token;

class LoxPlus
{
public:
    LoxPlus() = delete;

    static int runFile(const char*  name);
    static void runPrompt();
    static void run(std::string_view source);

    static void error(std::size_t line, std::string_view message);
    static void error(const Token & token, std::string_view message);

    static void runtimeError(const RuntimeError & error);

private:
    static void report(std::size_t line, std::string_view where, std::string_view message);

    static inline bool hadError = false;
    static inline bool hadRuntimeError = false;
};


#endif //LOXPLUS_LOXPLUS_H
