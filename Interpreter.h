//
// Created by minirop on 05/11/17.
//

#ifndef LOXPLUS_INTERPRETER_H
#define LOXPLUS_INTERPRETER_H

#include <stack>
#include <vector>
#include <map>
#include "ast.h"
#include "Environment.h"

class Interpreter : public VisitorExpr, public VisitorStmt
{
public:
    Interpreter();
    Interpreter(const Interpreter&) = delete;
    Interpreter(Interpreter&&) = default;
    Interpreter& operator=(const Interpreter&) = delete;
    Interpreter& operator=(Interpreter&&) = default;

    void visitAssignExpr(AssignExpr & expr) override;
    void visitBinaryExpr(BinaryExpr & expr) override;
    void visitCallExpr(CallExpr & expr) override;
    void visitGetExpr(GetExpr & expr) override;
    void visitGroupingExpr(GroupingExpr & expr) override;
    void visitLiteralExpr(LiteralExpr & expr) override;
    void visitLogicalExpr(LogicalExpr & expr) override;
    void visitSetExpr(SetExpr & expr) override;
    void visitThisExpr(ThisExpr & expr) override;
    void visitUnaryExpr(UnaryExpr & expr) override;
    void visitVariableExpr(VariableExpr & expr) override;
    void visitWhileStmt(WhileStmt & stmt) override;

    void visitBlockStmt(BlockStmt & stmt) override;
    void visitClassStmt(ClassStmt & stmt) override;
    void visitExpressionStmt(ExpressionStmt & stmt) override;
    void visitFunctionStmt(FunctionStmt & stmt) override;
    void visitIfStmt(IfStmt & stmt) override;
    void visitPrintStmt(PrintStmt & stmt) override;
    void visitReturnStmt(ReturnStmt & stmt) override;
    void visitVarStmt(VarStmt & stmt) override;

    void interpret(const std::vector<std::unique_ptr<Stmt>> & statements);

    void resolve(Expr & expr, unsigned long depth);

private:
    std::stack<Object> stack;
    std::shared_ptr<Environment> globals = std::make_shared<Environment>();
    std::shared_ptr<Environment> environment = globals;
    std::map<Expr*, unsigned long> locals;

    Object evaluate(Expr* expr);
    Object evaluate(const std::unique_ptr<Expr> & expr) { return evaluate(expr.get()); }

    bool isTruthy(Object object);
    bool isEqual(Object left, Object right);

    void execute(Stmt* stmt);
    void execute(const std::unique_ptr<Stmt> & stmt) { execute(stmt.get()); }
    void executeBlock(const std::vector<std::unique_ptr<Stmt>> & statements, std::shared_ptr<Environment> environment);

    friend class LoxFunction;

    void lookUpVariable(Token name, Expr & expr);
};

#endif //LOXPLUS_INTERPRETER_H
