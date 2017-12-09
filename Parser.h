//
// Created by minirop on 05/11/17.
//

#ifndef LOXPLUS_PARSER_H
#define LOXPLUS_PARSER_H

#include <vector>
#include <memory>
#include "Token.h"
#include "ast.h"

class Parser
{
public:
    explicit Parser(std::vector<Token> tokens);

    std::vector<std::unique_ptr<Stmt>> parse();

private:
    std::unique_ptr<Expr> assignment();
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> addition();
    std::unique_ptr<Expr> multiplication();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();
    std::unique_ptr<Expr> or_();
    std::unique_ptr<Expr> and_();
    std::unique_ptr<Expr> call();

    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Stmt> declaration();
    std::unique_ptr<Stmt> classDeclaration();
    std::unique_ptr<Stmt> expressionStatement();
    std::unique_ptr<Stmt> forStatement();
    std::unique_ptr<Stmt> ifStatement();
    std::unique_ptr<Stmt> printStatement();
    std::unique_ptr<Stmt> returnStatement();
    std::unique_ptr<Stmt> varDeclaration();
    std::unique_ptr<Stmt> whileStatement();

    std::vector<std::unique_ptr<Stmt>> block();

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

    std::unique_ptr<Expr> finishCall(std::unique_ptr<Expr> callee);
    std::unique_ptr<FunctionStmt> function(std::string kind);

};

#endif //LOXPLUS_PARSER_H
