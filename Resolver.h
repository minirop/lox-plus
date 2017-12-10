//
// Created by minirop on 01/12/17.
//

#ifndef LOXPLUS_RESOLVER_H
#define LOXPLUS_RESOLVER_H


#include <memory>
#include "ast.h"
#include "Interpreter.h"

class Resolver : public VisitorExpr, public VisitorStmt
{
public:
    explicit Resolver(Interpreter & interpreter);

    void visitAssignExpr(AssignExpr & expr) override;
    void visitBinaryExpr(BinaryExpr & expr) override;
    void visitCallExpr(CallExpr & expr) override;
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
    void visitGetExpr(GetExpr & expr) override;
    void visitExpressionStmt(ExpressionStmt & stmt) override;
    void visitFunctionStmt(FunctionStmt & stmt) override;
    void visitIfStmt(IfStmt & stmt) override;
    void visitPrintStmt(PrintStmt & stmt) override;
    void visitReturnStmt(ReturnStmt & stmt) override;
    void visitVarStmt(VarStmt & stmt) override;

    void resolve(const std::vector<Stmt*> & statements);

private:
    enum class FunctionType
    {
        None,
        Function,
        Initializer,
        Method
    };

    enum class ClassType
    {
        None,
        Class
    };

    Interpreter & interpreter;
    std::vector<std::map<std::string, bool>> scopes;
    FunctionType currentFunction = FunctionType::None;
    ClassType currentClass = ClassType::None;

    void beginScope();

    void endScope();

    void resolve(Stmt* stmt);
    void resolve(Expr* expr);

    void declare(Token name);

    void define(Token name);

    void resolveLocal(Expr & expr, Token name);

    void resolveFunction(FunctionStmt & stmt, FunctionType type);
};


#endif //LOXPLUS_RESOLVER_H
