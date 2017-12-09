//
// Created by minirop on 05/11/17.
//

#include <string_view>
#include "Lox-plus.h"
#include "ParseError.h"
#include "Parser.h"

Parser::Parser(std::vector<Token> tokens)
    : tokens { tokens }
{

}

std::unique_ptr<Expr> Parser::expression()
{
    return assignment();
}

std::unique_ptr<Expr> Parser::equality()
{
    auto expr = comparison();

    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL}))
    {
        auto op = previous();
        auto right = comparison();
        expr = std::make_unique<BinaryExpr>(std::move(expr), std::move(op), std::move(right));
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

std::unique_ptr<Expr> Parser::comparison()
{
    auto expr = addition();

    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL}))
    {
        auto op = previous();
        auto right = addition();
        expr = std::make_unique<BinaryExpr>(std::move(expr), std::move(op), std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::addition()
{
    auto expr = multiplication();

    while (match({TokenType::MINUS, TokenType::PLUS}))
    {
        auto op = previous();
        auto right = multiplication();
        expr = std::make_unique<BinaryExpr>(std::move(expr), std::move(op), std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::multiplication()
{
    auto expr = unary();

    while (match({TokenType::SLASH, TokenType::STAR}))
    {
        auto op = previous();
        auto right = unary();
        expr = std::make_unique<BinaryExpr>(std::move(expr), std::move(op), std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::unary()
{
    if (match({TokenType::BANG, TokenType::MINUS}))
    {
        auto op = previous();
        auto right = unary();
        return std::make_unique<UnaryExpr>(std::move(op), std::move(right));
    }

    return call();
}

std::unique_ptr<Expr> Parser::primary()
{
    if (match(TokenType::FALSE)) return std::make_unique<LiteralExpr>(false);
    if (match(TokenType::TRUE)) return std::make_unique<LiteralExpr>(true);
    if (match(TokenType::NIL)) return std::make_unique<LiteralExpr>(nullptr);

    if (match({TokenType::NUMBER, TokenType::STRING}))
    {
        return std::make_unique<LiteralExpr>(previous().literal);
    }

    if (match(TokenType::THIS)) return std::make_unique<ThisExpr>(previous());

    if (match(TokenType::IDENTIFIER))
    {
        return std::make_unique<VariableExpr>(previous());
    }

    if (match(TokenType::LEFT_PAREN))
    {
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_unique<GroupingExpr>(std::move(expr));
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

std::vector<std::unique_ptr<Stmt>> Parser::parse()
{
    try
    {
        std::vector<std::unique_ptr<Stmt>> statements;

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

std::unique_ptr<Stmt> Parser::statement()
{
    if (match(TokenType::FUN)) return function("function");
    if (match(TokenType::IF)) return ifStatement();
    if (match(TokenType::WHILE)) return whileStatement();
    if (match(TokenType::FOR)) return forStatement();
    if (match(TokenType::PRINT)) return printStatement();
    if (match(TokenType::RETURN)) return returnStatement();
    if (match(TokenType::LEFT_BRACE)) return std::make_unique<BlockStmt>(block());

    return expressionStatement();
}

std::unique_ptr<Stmt> Parser::printStatement()
{
    auto value = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    return std::make_unique<PrintStmt>(std::move(value));
}

std::unique_ptr<Stmt> Parser::expressionStatement()
{
    auto expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    return std::make_unique<ExpressionStmt>(std::move(expr));
}

std::unique_ptr<Stmt> Parser::declaration()
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

std::unique_ptr<Stmt> Parser::varDeclaration()
{
    auto name = consume(TokenType::IDENTIFIER, "Expect variable name.");

    std::unique_ptr<Expr> initializer;
    if (match(TokenType::EQUAL))
    {
        initializer = expression();
    }

    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_unique<VarStmt>(name, std::move(initializer));
}

std::unique_ptr<Expr> Parser::assignment()
{
    auto expr = or_();

    if (match(TokenType::EQUAL))
    {
        auto equals = previous();
        auto value = assignment();

        if (auto * varExpr = dynamic_cast<VariableExpr*>(expr.get()); varExpr != nullptr)
        {
            expr.release();
            return std::make_unique<AssignExpr>(std::move(varExpr->name), std::move(value));
        }
        else if (auto * getExpr = dynamic_cast<GetExpr*>(expr.get()); getExpr != nullptr)
        {
            expr.release();
            return std::make_unique<SetExpr>(std::move(getExpr->object), std::move(getExpr->name), std::move(value));
        }

        error(equals, "Invalid assignment target.");
    }

    return expr;
}

std::vector<std::unique_ptr<Stmt>> Parser::block()
{
    std::vector<std::unique_ptr<Stmt>> statements;

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd())
    {
        statements.push_back(declaration());
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

std::unique_ptr<Stmt> Parser::ifStatement()
{
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
    auto condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");

    auto thenBranch = statement();
    std::unique_ptr<Stmt> elseBranch = nullptr;
    if (match(TokenType::ELSE))
    {
        elseBranch = statement();
    }

    return std::make_unique<IfStmt>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

std::unique_ptr<Expr> Parser::or_()
{
    auto expr = and_();

    while (match(TokenType::OR))
    {
        auto op = previous();
        auto right = and_();
        expr = std::make_unique<LogicalExpr>(std::move(expr), std::move(op), std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::and_()
{
    auto expr = equality();

    while (match(TokenType::AND))
    {
        auto op = previous();
        auto right = equality();
        expr = std::make_unique<LogicalExpr>(std::move(expr), std::move(op), std::move(right));
    }

    return expr;
}

std::unique_ptr<Stmt> Parser::whileStatement()
{
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
    auto condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
    auto body = statement();

    return std::make_unique<WhileStmt>(std::move(condition), std::move(body));
}

std::unique_ptr<Stmt> Parser::forStatement()
{
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'.");

    std::unique_ptr<Stmt> initializer = nullptr;
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

    std::unique_ptr<Expr> condition;
    if (!check(TokenType::SEMICOLON))
    {
        condition = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after loop condition.");

    std::unique_ptr<Expr> increment;
    if (!check(TokenType::RIGHT_PAREN))
    {
        increment = expression();
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");

    std::unique_ptr<Stmt> body = statement();

    if (increment != nullptr)
    {
        std::vector<std::unique_ptr<Stmt>> newBody;
        newBody.push_back(std::move(body));
        newBody.push_back(std::make_unique<ExpressionStmt>(std::move(increment)));
        body = std::make_unique<BlockStmt>(std::move(newBody));
    }

    if (condition == nullptr) condition = std::make_unique<LiteralExpr>(true);
    body = std::make_unique<WhileStmt>(std::move(condition), std::move(body));

    if (initializer != nullptr)
    {
        std::vector<std::unique_ptr<Stmt>> newBody;
        newBody.push_back(std::move(initializer));
        newBody.push_back(std::move(body));
        body = std::make_unique<BlockStmt>(std::move(newBody));
    }

    return body;
}

std::unique_ptr<Expr> Parser::call()
{
    auto expr = primary();

    while (true)
    {
        if (match(TokenType::LEFT_PAREN))
        {
            expr = finishCall(std::move(expr));
        }
        else if (match(TokenType::DOT))
        {
            Token name = consume(TokenType::IDENTIFIER, "Expect property name after '.'.");
            expr = std::make_unique<GetExpr>(std::move(expr), std::move(name));
        }
        else
        {
            break;
        }
    }

    return expr;
}

std::unique_ptr<Expr> Parser::finishCall(std::unique_ptr<Expr> callee)
{
    std::vector<std::unique_ptr<Expr>> arguments;
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

    return std::make_unique<CallExpr>(std::move(callee), std::move(paren), std::move(arguments));
}

std::unique_ptr<FunctionStmt> Parser::function(std::string kind)
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
    return std::make_unique<FunctionStmt>(std::move(name), std::move(parameters), std::move(body));
}

std::unique_ptr<Stmt> Parser::returnStatement()
{
    auto keyword = previous();
    std::unique_ptr<Expr> value;
    if (!check(TokenType::SEMICOLON))
    {
        value = expression();
    }

    consume(TokenType::SEMICOLON, "Expect ';' after return value.");
    return std::make_unique<ReturnStmt>(std::move(keyword), std::move(value));
}

std::unique_ptr<Stmt> Parser::classDeclaration()
{
    Token name = consume(TokenType::IDENTIFIER, "Expect class name.");
    consume(TokenType::LEFT_BRACE, "Expect '{' before class body.");

    std::vector<std::unique_ptr<FunctionStmt>> methods;
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd())
    {
        methods.push_back(function("method"));
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after class body.");

    return std::make_unique<ClassStmt>(std::move(name), std::move(methods));
}
