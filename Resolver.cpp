//
// Created by minirop on 01/12/17.
//

#include <iostream>
#include "Resolver.h"
#include "Lox-plus.h"

Resolver::Resolver(Interpreter & interpreter)
    : interpreter { interpreter }
{

}

void Resolver::visitAssignExpr(AssignExpr & expr)
{
    resolve(expr.value);
    resolveLocal(expr, expr.name);
}

void Resolver::visitBinaryExpr(BinaryExpr & expr)
{
    resolve(expr.left);
    resolve(expr.right);
}

void Resolver::visitCallExpr(CallExpr & expr)
{
    resolve(expr.callee);

    for (auto & argument : expr.arguments)
    {
        resolve(argument);
    }
}

void Resolver::visitGetExpr(GetExpr & expr)
{
    resolve(expr.object);
}

void Resolver::visitGroupingExpr(GroupingExpr & expr)
{
    resolve(expr.expression);
}

void Resolver::visitLiteralExpr(LiteralExpr & expr)
{
}

void Resolver::visitLogicalExpr(LogicalExpr & expr)
{
    resolve(expr.left);
    resolve(expr.right);
}

void Resolver::visitThisExpr(ThisExpr & expr)
{
    if (currentClass == ClassType::None)
    {
        LoxPlus::error(expr.keyword, "Cannot use 'this' outside of a class.");
        return;
    }

    resolveLocal(expr, expr.keyword);
}

void Resolver::visitUnaryExpr(UnaryExpr & expr)
{
    resolve(expr.right);
}

void Resolver::visitSetExpr(SetExpr & expr)
{
    resolve(expr.value);
    resolve(expr.object);
}

void Resolver::visitVariableExpr(VariableExpr & expr)
{
    if (!scopes.empty() && scopes.back()[expr.name.lexeme] == false)
    {
        LoxPlus::error(expr.name, "Cannot read local variable in its own initializer.");
    }

    resolveLocal(expr, expr.name);
}

void Resolver::visitBlockStmt(BlockStmt & stmt)
{
    beginScope();
    resolve(stmt.statements);
    endScope();
}

void Resolver::visitClassStmt(ClassStmt & stmt)
{
    declare(stmt.name);
    define(stmt.name);

    ClassType enclosingClass = currentClass;
    currentClass = ClassType::Class;

    beginScope();
    scopes.back()["this"] = true;

    for (auto & method : stmt.methods)
    {
        FunctionType declaration = FunctionType::Method;
        if (method->name.lexeme == "init")
        {
            declaration = FunctionType::Initializer;
        }
        resolveFunction(*method, declaration);
    }

    endScope();

    currentClass = enclosingClass;
}

void Resolver::visitExpressionStmt(ExpressionStmt & stmt)
{
    resolve(stmt.expression);
}

void Resolver::visitFunctionStmt(FunctionStmt & stmt)
{
    declare(stmt.name);
    define(stmt.name);

    resolveFunction(stmt, FunctionType::Function);
}

void Resolver::visitIfStmt(IfStmt & stmt)
{
    resolve(stmt.condition);
    resolve(stmt.thenBranch);
    if (stmt.elseBranch != nullptr) resolve(stmt.elseBranch);
}

void Resolver::visitPrintStmt(PrintStmt & stmt)
{
    resolve(stmt.expression);
}

void Resolver::visitReturnStmt(ReturnStmt & stmt)
{
    if (currentFunction == FunctionType::None)
    {
        LoxPlus::error(stmt.keyword, "Cannot return from top-level code.");
    }

    if (stmt.value != nullptr)
    {
        if (currentFunction == FunctionType::Initializer)
        {
            LoxPlus::error(stmt.keyword, "Cannot return a value from an initializer.");
        }

        resolve(stmt.value);
    }
}

void Resolver::visitVarStmt(VarStmt & stmt)
{
    declare(stmt.name);
    if (stmt.initializer != nullptr)
    {
        resolve(stmt.initializer);
    }
    define(stmt.name);
}

void Resolver::visitWhileStmt(WhileStmt & stmt)
{
    resolve(stmt.condition);
    resolve(stmt.body);
}

void Resolver::beginScope()
{
    scopes.emplace_back();
}

void Resolver::endScope()
{
    scopes.pop_back();
}

void Resolver::resolve(const std::vector<std::unique_ptr<Stmt>> & statements)
{
    for (auto & statement : statements)
    {
        resolve(statement);
    }
}

void Resolver::resolve(Stmt * stmt)
{
    stmt->accept(*this);
}

void Resolver::resolve(Expr * expr)
{
    expr->accept(*this);
}

void Resolver::declare(Token name)
{
    if (scopes.empty()) return;

    auto & scope = scopes.back();
    if (scope.count(name.lexeme))
    {
        LoxPlus::error(name, "Variable with this name already declared in this scope.");
    }

    scope[name.lexeme] = false;
}

void Resolver::define(Token name)
{
    if (scopes.empty()) return;
    scopes.back()[name.lexeme] = true;
}

void Resolver::resolveLocal(Expr & expr, Token name)
{
    auto size = static_cast<long int>(scopes.size());
    for (auto i = size - 1; i >= 0; i--)
    {
        if (scopes[i].count(name.lexeme))
        {
            interpreter.resolve(expr, scopes.size() - 1 - i);
            return;
        }
    }
}

void Resolver::resolveFunction(FunctionStmt & function, FunctionType type)
{
    auto enclosingFunction = currentFunction;
    currentFunction = type;

    beginScope();
    for (auto & param : function.parameters)
    {
        declare(param);
        define(param);
    }
    resolve(function.body);
    endScope();

    currentFunction = enclosingFunction;
}
