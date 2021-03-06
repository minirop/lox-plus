//
// Created by minirop on 05/11/17.
//

#include <string_view>
#include "Lox-plus.h"
#include "ParseError.h"
#include "Parser.h"

Parser::Parser(std::vector<Token> tokens)
    : tokens { std::move(tokens) }
{

}

Expr* Parser::expression()
{
    return assignment();
}

Expr* Parser::equality()
{
    auto expr = comparison();

    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL}))
    {
        auto op = previous();
        auto right = comparison();
        expr = BinaryExpr::create(expr, op, right);
    }

    return expr;
}

bool Parser::match(TokenType type)
{
    if (check(type))
    {
        advance();
        return true;
    }

    return false;
}

bool Parser::match(std::vector<TokenType> types)
{
    for (auto & type : types)
    {
        if (match(type))
        {
            return true;
        }
    }

    return false;
}

bool Parser::check(TokenType tokenType)
{
    if (isAtEnd()) return false;
    return peek().type == tokenType;
}

const Token & Parser::advance()
{
    if (!isAtEnd()) current++;
    return previous();
}

#undef EOF

bool Parser::isAtEnd()
{
    return peek().type == TokenType::EOF;
}

const Token & Parser::peek()
{
    return tokens.at(current);
}

const Token & Parser::previous()
{
    return tokens.at(current - 1);
}

Expr* Parser::comparison()
{
    auto expr = addition();

    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL}))
    {
        auto op = previous();
        auto right = addition();
        expr = BinaryExpr::create(expr, op, right);
    }

    return expr;
}

Expr* Parser::addition()
{
    auto expr = multiplication();

    while (match({TokenType::MINUS, TokenType::PLUS}))
    {
        auto op = previous();
        auto right = multiplication();
        expr = BinaryExpr::create(expr, op, right);
    }

    return expr;
}

Expr* Parser::multiplication()
{
    auto expr = unary();

    while (match({TokenType::SLASH, TokenType::STAR}))
    {
        auto op = previous();
        auto right = unary();
        expr = BinaryExpr::create(expr, op, right);
    }

    return expr;
}

Expr* Parser::unary()
{
    if (match({TokenType::BANG, TokenType::MINUS}))
    {
        auto op = previous();
        auto right = unary();
        return UnaryExpr::create(op, right);
    }

    return call();
}

Expr* Parser::primary()
{
    if (match(TokenType::FALSE)) return LiteralExpr::create(false);
    if (match(TokenType::TRUE)) return LiteralExpr::create(true);
    if (match(TokenType::NIL)) return LiteralExpr::create(nullptr);

    if (match({TokenType::NUMBER, TokenType::STRING}))
    {
        return LiteralExpr::create(previous().literal);
    }

    if (match(TokenType::THIS)) return ThisExpr::create(previous());

    if (match(TokenType::IDENTIFIER))
    {
        return VariableExpr::create(previous());
    }

    if (match(TokenType::LEFT_PAREN))
    {
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return GroupingExpr::create(std::move(expr));
    }

    error(peek(), "Expect expression.");
}

const Token & Parser::consume(TokenType type, std::string_view message)
{
    if (check(type)) return advance();

    error(peek(), message);
}

void Parser::error(const Token & token, std::string_view message)
{
    LoxPlus::error(token, message);
    throw ParseError();
}

void Parser::synchronize()
{
    advance();

    while (!isAtEnd())
    {
        if (previous().type == TokenType::SEMICOLON) return;

        switch (peek().type)
        {
            case TokenType::CLASS:
            case TokenType::FUN:
            case TokenType::VAR:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::PRINT:
            case TokenType::RETURN:
                return;
            default:
                break;
        }

        advance();
    }
}

std::vector<Stmt*> Parser::parse()
{
    try
    {
        std::vector<Stmt*> statements;

        while (!isAtEnd())
        {
            statements.push_back(declaration());
        }

        return statements;
    }
    catch (const ParseError & error)
    {
        return {};
    }
}

Stmt* Parser::statement()
{
    if (match(TokenType::FUN)) return function("function");
    if (match(TokenType::IF)) return ifStatement();
    if (match(TokenType::WHILE)) return whileStatement();
    if (match(TokenType::FOR)) return forStatement();
    if (match(TokenType::PRINT)) return printStatement();
    if (match(TokenType::RETURN)) return returnStatement();
    if (match(TokenType::LEFT_BRACE)) return BlockStmt::create(block());

    return expressionStatement();
}

Stmt* Parser::printStatement()
{
    auto value = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    return PrintStmt::create(value);
}

Stmt* Parser::expressionStatement()
{
    auto expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    return ExpressionStmt::create(expr);
}

Stmt* Parser::declaration()
{
    try
    {
        if (match(TokenType::CLASS)) return classDeclaration();
        if (match(TokenType::VAR)) return varDeclaration();

        return statement();
    }
    catch (const ParseError& error)
    {
        synchronize();
        return nullptr;
    }
}

Stmt* Parser::varDeclaration()
{
    auto name = consume(TokenType::IDENTIFIER, "Expect variable name.");

    Expr* initializer;
    if (match(TokenType::EQUAL))
    {
        initializer = expression();
    }

    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return VarStmt::create(name, initializer);
}

Expr* Parser::assignment()
{
    auto expr = or_();

    if (match(TokenType::EQUAL))
    {
        auto equals = previous();
        auto value = assignment();

        if (auto* varExpr = dynamic_cast<VariableExpr*>(expr); varExpr != nullptr)
        {
            return AssignExpr::create(std::move(varExpr->name), std::move(value));
        }
        else if (auto* getExpr = dynamic_cast<GetExpr*>(expr); getExpr != nullptr)
        {
            return SetExpr::create(getExpr->object, getExpr->name, value);
        }

        error(equals, "Invalid assignment target.");
    }

    return expr;
}

std::vector<Stmt*> Parser::block()
{
    std::vector<Stmt*> statements;

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd())
    {
        statements.push_back(declaration());
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

Stmt* Parser::ifStatement()
{
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
    auto condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");

    auto thenBranch = statement();
    Stmt* elseBranch = nullptr;
    if (match(TokenType::ELSE))
    {
        elseBranch = statement();
    }

    return IfStmt::create(condition, thenBranch, elseBranch);
}

Expr* Parser::or_()
{
    auto expr = and_();

    while (match(TokenType::OR))
    {
        auto op = previous();
        auto right = and_();
        expr = LogicalExpr::create(expr, op, right);
    }

    return expr;
}

Expr* Parser::and_()
{
    auto expr = equality();

    while (match(TokenType::AND))
    {
        auto op = previous();
        auto right = equality();
        expr = LogicalExpr::create(expr, op, right);
    }

    return expr;
}

Stmt* Parser::whileStatement()
{
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
    auto condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
    auto body = statement();

    return WhileStmt::create(condition, body);
}

Stmt* Parser::forStatement()
{
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'.");

    Stmt* initializer = nullptr;
    if (match(TokenType::SEMICOLON))
    {
        initializer = nullptr;
    }
    else if (match(TokenType::VAR))
    {
        initializer = varDeclaration();
    }
    else
    {
        initializer = expressionStatement();
    }

    Expr* condition = nullptr;
    if (!check(TokenType::SEMICOLON))
    {
        condition = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after loop condition.");

    Expr* increment = nullptr;
    if (!check(TokenType::RIGHT_PAREN))
    {
        increment = expression();
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");

    Stmt* body = statement();

    if (increment != nullptr)
    {
        std::vector<Stmt*> newBody;
        newBody.push_back(body);
        newBody.push_back(ExpressionStmt::create(increment));
        body = BlockStmt::create(newBody);
    }

    if (condition == nullptr) condition = LiteralExpr::create(true);
    body = WhileStmt::create(condition, body);

    if (initializer != nullptr)
    {
        std::vector<Stmt*> newBody;
        newBody.push_back(initializer);
        newBody.push_back(body);
        body = BlockStmt::create(newBody);
    }

    return body;
}

Expr* Parser::call()
{
    auto expr = primary();

    while (true)
    {
        if (match(TokenType::LEFT_PAREN))
        {
            expr = finishCall(expr);
        }
        else if (match(TokenType::DOT))
        {
            Token name = consume(TokenType::IDENTIFIER, "Expect property name after '.'.");
            expr = GetExpr::create(expr, name);
        }
        else
        {
            break;
        }
    }

    return expr;
}

Expr* Parser::finishCall(Expr* callee)
{
    std::vector<Expr*> arguments;
    if (!check(TokenType::RIGHT_PAREN))
    {
        do
        {
            if (arguments.size() >= 8)
            {
                error(peek(), "Cannot have more than 8 arguments.");
            }
            arguments.push_back(expression());
        } while (match(TokenType::COMMA));
    }

    Token paren = consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");

    return CallExpr::create(callee, paren, arguments);
}

FunctionStmt* Parser::function(std::string kind)
{
    Token name = consume(TokenType::IDENTIFIER, "Expect " + kind + " name.");

    consume(TokenType::LEFT_PAREN, "Expect '(' after " + kind + " name.");
    std::vector<Token> parameters;
    if (!check(TokenType::RIGHT_PAREN))
    {
        do
        {
            if (parameters.size() >= 8)
            {
                error(peek(), "Cannot have more than 8 parameters.");
            }

            parameters.push_back(consume(TokenType::IDENTIFIER, "Expect parameter name."));
        } while (match(TokenType::COMMA));
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");

    consume(TokenType::LEFT_BRACE, "Expect '{' before " + kind + " body.");
    auto body = block();
    return FunctionStmt::create(name, parameters, body);
}

Stmt* Parser::returnStatement()
{
    auto keyword = previous();
    Expr* value = nullptr;
    if (!check(TokenType::SEMICOLON))
    {
        value = expression();
    }

    consume(TokenType::SEMICOLON, "Expect ';' after return value.");
    return ReturnStmt::create(keyword, value);
}

Stmt* Parser::classDeclaration()
{
    Token name = consume(TokenType::IDENTIFIER, "Expect class name.");
    consume(TokenType::LEFT_BRACE, "Expect '{' before class body.");

    std::vector<FunctionStmt*> methods;
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd())
    {
        methods.push_back(function("method"));
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after class body.");

    return ClassStmt::create(name, methods);
}
