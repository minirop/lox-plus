//
// Created by minirop on 05/11/17.
//

#ifndef LOXPLUS_PARSER_H
#define LOXPLUS_PARSER_H

#include <vector>
#include "Token.h"
#include "ast.h"

class Parser
{
public:
    explicit Parser(std::vector<Token> tokens);

    std::vector<Stmt*> parse();

private:
    Expr* assignment();
    Expr* expression();
    Expr* equality();
    Expr* comparison();
    Expr* addition();
    Expr* multiplication();
    Expr* unary();
    Expr* primary();
    Expr* or_();
    Expr* and_();
    Expr* call();

    Stmt* statement();
    Stmt* declaration();
    Stmt* classDeclaration();
    Stmt* expressionStatement();
    Stmt* forStatement();
    Stmt* ifStatement();
    Stmt* printStatement();
    Stmt* returnStatement();
    Stmt* varDeclaration();
    Stmt* whileStatement();

    std::vector<Stmt *> block();

    const Token & advance();

    bool check(TokenType tokenType);
    bool match(TokenType type);
    bool match(std::vector<TokenType> types);
    bool isAtEnd();

    const Token & peek();
    const Token & previous();
    const Token & consume(TokenType type, std::string_view message);

    void error(const Token & token, std::string_view message);
    void synchronize();

    std::vector<Token> tokens;
    std::size_t current = 0;

    Expr * finishCall(Expr * callee);
    FunctionStmt * function(std::string kind);

};

#endif //LOXPLUS_PARSER_H
