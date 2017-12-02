//
// Created by minirop on 05/11/17.
//

#include "Interpreter.h"
#include "Lox-plus.h"
#include "LoxCallable.h"
#include "LoxFunction.h"
#include "LoxInstance.h"
#include "Return.h"
#include "LoxClass.h"
#include <iostream>

using namespace std::string_literals;

Interpreter::Interpreter()
{
    //globals.define("clock", ...);
}

void Interpreter::visitAssignExpr(AssignExpr & expr)
{
    Object value = evaluate(expr.value);

    if (locals.count(&expr))
    {
        auto distance = locals[&expr];
        environment->assignAt(distance, expr.name, value);
    }
    else
    {
        globals.assign(expr.name, value);
    }

    environment->assign(expr.name, value);
    stack.push(value);
}

void Interpreter::visitBinaryExpr(BinaryExpr & expr)
{
    auto left = evaluate(expr.left);
    auto right = evaluate(expr.right);

    if (left.index() != right.index())
    {
        throw RuntimeError(expr.op, "Binary operator work on operands of the same type.");
    }

    Object result;

    if (left.isDouble())
    {
        double l = left.asDouble();
        double r = right.asDouble();

        switch (expr.op.type)
        {
            case TokenType::PLUS:
                result = l + r;
                break;
            case TokenType::MINUS:
                result = l - r;
                break;
            case TokenType::SLASH:
                result = l / r;
                break;
            case TokenType::STAR:
                result = l * r;
                break;

            case TokenType::GREATER:
                result = l > r;
                break;
            case TokenType::GREATER_EQUAL:
                result = l >= r;
                break;
            case TokenType::LESS:
                result = l < r;
                break;
            case TokenType::LESS_EQUAL:
                result = l <= r;
                break;
            case TokenType::BANG_EQUAL:
                result = !isEqual(left, right);
                break;
            case TokenType::EQUAL_EQUAL:
                result = isEqual(left, right);
                break;
            default:
                throw RuntimeError(expr.op, "Unknown binary operator for integer.");
        }
    }
    else if (left.isString())
    {
        std::string l = left.asString();
        std::string r = right.asString();

        switch (expr.op.type)
        {
            case TokenType::PLUS:
                result = l + r;
                break;
            case TokenType::BANG_EQUAL:
                result = !isEqual(left, right);
                break;
            case TokenType::EQUAL_EQUAL:
                result = isEqual(left, right);
                break;
            default:
                throw RuntimeError(expr.op, "Unknown binary operator for strings.");
        }
    }
    else
    {
        switch (expr.op.type)
        {
            case TokenType::BANG_EQUAL:
                result = !isEqual(left, right);
                break;
            case TokenType::EQUAL_EQUAL:
                result = isEqual(left, right);
                break;
            default:
                throw RuntimeError(expr.op, "Unknown binary operator.");
        }
    }

    stack.push(result);
}

void Interpreter::visitCallExpr(CallExpr & expr)
{
    Object callee = evaluate(expr.callee);

    std::vector<Object> arguments;
    for (auto & argument : expr.arguments)
    {
        arguments.push_back(evaluate(argument));
    }

    if (!callee.isCallable())
    {
        throw RuntimeError(expr.paren, "Can only call functions and classes.");
    }

    auto function = callee.asCallable();
    if (arguments.size() != function->arity())
    {
        throw RuntimeError(expr.paren, "Expected "s + std::to_string(function->arity()) + " arguments but got "s + std::to_string(arguments.size()) + ".");
    }
    stack.push(function->call(*this, arguments));
}

void Interpreter::visitGetExpr(GetExpr & expr)
{
    Object object = evaluate(expr.object);
    if (!object.isInstance())
    {
        throw RuntimeError(expr.name, "Only instances have properties.");
    }

    stack.push(object.asInstance()->get(expr.name));
}

void Interpreter::visitGroupingExpr(GroupingExpr & expr)
{
    stack.push(evaluate(expr.expression));
}

void Interpreter::visitLiteralExpr(LiteralExpr & expr)
{
    stack.push(expr.value);
}

void Interpreter::visitLogicalExpr(LogicalExpr & expr)
{
    Object left = evaluate(expr.left);

    /*
     * simplified from :
     *     if (expr.op.type == TokenType::OR &&  isTruthy(left)) { return left; }
     *     if (expr.op.type != TokenType::OR && !isTruthy(left)) { return left; }
     *
     * */
    if ((expr.op.type == TokenType::OR) == isTruthy(left))
    {
        stack.push(left);
        return;
    }

    stack.push(evaluate(expr.right));
}

void Interpreter::visitSetExpr(SetExpr & expr)
{
    Object object = evaluate(expr.object);

    if (!(object.isInstance()))
    {
        throw RuntimeError(expr.name, "Only instances have fields.");
    }

    Object value = evaluate(expr.value);
    object.asInstance()->set(expr.name, value);
    stack.push(value);
}

void Interpreter::visitThisExpr(ThisExpr & expr)
{
    lookUpVariable(expr.keyword, expr);
}

void Interpreter::visitUnaryExpr(UnaryExpr & expr)
{
    Object right = evaluate(expr.right);

    switch (expr.op.type)
    {
        case TokenType::MINUS:
            if (!right.isDouble())
            {
                throw RuntimeError(expr.op, "Operand must be a number.");
            }
            stack.push(-1 * right.asDouble());
            break;
        case TokenType::BANG:
            stack.push(!isTruthy(right));
            break;
        default:
            throw RuntimeError(expr.op, "Unknown unary operator.");
    }
}

void Interpreter::visitVariableExpr(VariableExpr & expr)
{
    lookUpVariable(expr.name, expr);
}

bool Interpreter::isTruthy(Object object)
{
    bool ret = true;

    if (object.isNull())
    {
        ret = false;
    }
    else if (object.isBool())
    {
        ret = object.asBool();
    }

    return ret;
}

bool Interpreter::isEqual(Object left, Object right)
{
    if (left.index() == right.index())
    {
        if (left.isString())
        {
            return left.asString() == right.asString();
        }
        else if (left.isDouble())
        {
            return left.asDouble() == right.asDouble();
        }
        else if (left.isNull())
        {
            return true;
        }
        else if (left.isBool())
        {
            return left.asBool() == right.asBool();
        }
    }

    return false;
}

Object Interpreter::evaluate(Expr * expr)
{
    expr->accept(*this);
    auto s = stack.top();
    stack.pop();
    return s;
}

void Interpreter::interpret(std::vector<Stmt*> statements)
{
    try
    {
        for (auto & statement : statements)
        {
            execute(statement);
        }
    }
    catch (const RuntimeError & error)
    {
        LoxPlus::runtimeError(error);
    }
}

void Interpreter::visitBlockStmt(BlockStmt & stmt)
{
    executeBlock(stmt.statements, new Environment(environment));
}

void Interpreter::visitClassStmt(ClassStmt & stmt)
{
    environment->define(stmt.name.lexeme, nullptr);

    std::map<std::string, LoxFunction*> methods;
    for (auto & method : stmt.methods)
    {
        auto function = new LoxFunction(method, environment, method->name.lexeme == "init");
        methods.emplace(method->name.lexeme, function);
    }

    auto klass = new LoxClass(stmt.name.lexeme, methods);
    environment->assign(stmt.name, klass);
}

void Interpreter::visitExpressionStmt(ExpressionStmt & stmt)
{
    evaluate(stmt.expression);
}

void Interpreter::visitFunctionStmt(FunctionStmt & stmt)
{
    auto function = new LoxFunction(&stmt, environment, false);
    environment->define(stmt.name.lexeme, function);
}

void Interpreter::visitIfStmt(IfStmt & stmt)
{
    if (isTruthy(evaluate(stmt.condition)))
    {
        execute(stmt.thenBranch);
    }
    else if (stmt.elseBranch != nullptr)
    {
        execute(stmt.elseBranch);
    }
}

void Interpreter::visitPrintStmt(PrintStmt & stmt)
{
    Object value = evaluate(stmt.expression);
    std::cout << to_string(value) << '\n';
}

void Interpreter::visitReturnStmt(ReturnStmt & stmt)
{
    Object value = nullptr;
    if (stmt.value != nullptr) value = evaluate(stmt.value);

    throw Return(value);
}

void Interpreter::visitVarStmt(VarStmt & stmt)
{
    Object value;
    if (stmt.initializer != nullptr)
    {
        value = evaluate(stmt.initializer);
    }

    environment->define(stmt.name.lexeme, value);
}

void Interpreter::visitWhileStmt(WhileStmt & stmt)
{
    while (isTruthy(evaluate(stmt.condition)))
    {
        execute(stmt.body);
    }
}

void Interpreter::execute(Stmt * stmt)
{
    stmt->accept(*this);
}

void Interpreter::executeBlock(std::vector<Stmt*> statements, Environment * environment)
{
    auto previous = this->environment;

    try
    {
        this->environment = environment;

        for (auto & statement : statements)
        {
            execute(statement);
        }
    }
    catch(...)
    {
        this->environment = previous;
        throw;
    }
    this->environment = previous;
}

void Interpreter::resolve(Expr & expr, unsigned long depth)
{
    locals[&expr] = depth;
}

void Interpreter::lookUpVariable(Token name, Expr & expr)
{
    if (locals.count(&expr))
    {
        auto distance = locals[&expr];
        stack.push(environment->getAt(distance, name.lexeme));
    }
    else
    {
        stack.push(globals.get(name));
    }
}
