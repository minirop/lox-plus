#ifndef LOXPLUS_AST_H
#define LOXPLUS_AST_H

#include "Token.h"
#include "Object.h"
#include "CreatableType.h"
#include <vector>

class AssignExpr;
class BinaryExpr;
class CallExpr;
class GetExpr;
class GroupingExpr;
class LiteralExpr;
class LogicalExpr;
class SetExpr;
class ThisExpr;
class UnaryExpr;
class VariableExpr;

struct VisitorExpr
{
	virtual ~VisitorExpr() = default;

	virtual void visitAssignExpr(AssignExpr & expr) = 0;
	virtual void visitBinaryExpr(BinaryExpr & expr) = 0;
	virtual void visitCallExpr(CallExpr & expr) = 0;
	virtual void visitGetExpr(GetExpr & expr) = 0;
	virtual void visitGroupingExpr(GroupingExpr & expr) = 0;
	virtual void visitLiteralExpr(LiteralExpr & expr) = 0;
	virtual void visitLogicalExpr(LogicalExpr & expr) = 0;
	virtual void visitSetExpr(SetExpr & expr) = 0;
	virtual void visitThisExpr(ThisExpr & expr) = 0;
	virtual void visitUnaryExpr(UnaryExpr & expr) = 0;
	virtual void visitVariableExpr(VariableExpr & expr) = 0;
};

struct Expr
{
	virtual void accept(VisitorExpr & visitor) = 0;
};

struct AssignExpr : CreatableType<AssignExpr>, Expr
{
	AssignExpr(Token name, Expr* value)
		: name { std::move(name) }, value { value }
	{
	}

	Token name;
	Expr* value;

	void accept(VisitorExpr & visitor) override
	{
		visitor.visitAssignExpr(*this);
	}
};

struct BinaryExpr : CreatableType<BinaryExpr>, Expr
{
	BinaryExpr(Expr* left, Token op, Expr* right)
		: left { left }, op { std::move(op) }, right { right }
	{
	}

	Expr* left;
	Token op;
	Expr* right;

	void accept(VisitorExpr & visitor) override
	{
		visitor.visitBinaryExpr(*this);
	}
};

struct CallExpr : CreatableType<CallExpr>, Expr
{
	CallExpr(Expr* callee, Token paren, std::vector<Expr*> arguments)
		: callee { callee }, paren { std::move(paren) }, arguments { std::move(arguments) }
	{
	}

	Expr* callee;
	Token paren;
	std::vector<Expr*> arguments;

	void accept(VisitorExpr & visitor) override
	{
		visitor.visitCallExpr(*this);
	}
};

struct GetExpr : CreatableType<GetExpr>, Expr
{
	GetExpr(Expr* object, Token name)
		: object { object }, name { std::move(name) }
	{
	}

	Expr* object;
	Token name;

	void accept(VisitorExpr & visitor) override
	{
		visitor.visitGetExpr(*this);
	}
};

struct GroupingExpr : CreatableType<GroupingExpr>, Expr
{
	explicit GroupingExpr(Expr* expression)
		: expression { expression }
	{
	}

	Expr* expression;

	void accept(VisitorExpr & visitor) override
	{
		visitor.visitGroupingExpr(*this);
	}
};

struct LiteralExpr : CreatableType<LiteralExpr>, Expr
{
	explicit LiteralExpr(Object value)
		: value { std::move(value) }
	{
	}

	Object value;

	void accept(VisitorExpr & visitor) override
	{
		visitor.visitLiteralExpr(*this);
	}
};

struct LogicalExpr : CreatableType<LogicalExpr>, Expr
{
	LogicalExpr(Expr* left, Token op, Expr* right)
		: left { left }, op { std::move(op) }, right { right }
	{
	}

	Expr* left;
	Token op;
	Expr* right;

	void accept(VisitorExpr & visitor) override
	{
		visitor.visitLogicalExpr(*this);
	}
};

struct SetExpr : CreatableType<SetExpr>, Expr
{
	SetExpr(Expr* object, Token name, Expr* value)
		: object { object }, name { std::move(name) }, value { value }
	{
	}

	Expr* object;
	Token name;
	Expr* value;

	void accept(VisitorExpr & visitor) override
	{
		visitor.visitSetExpr(*this);
	}
};

struct ThisExpr : CreatableType<ThisExpr>, Expr
{
	explicit ThisExpr(Token keyword)
		: keyword { std::move(keyword) }
	{
	}

	Token keyword;

	void accept(VisitorExpr & visitor) override
	{
		visitor.visitThisExpr(*this);
	}
};

struct UnaryExpr : CreatableType<UnaryExpr>, Expr
{
	UnaryExpr(Token op, Expr* right)
		: op { std::move(op) }, right { right }
	{
	}

	Token op;
	Expr* right;

	void accept(VisitorExpr & visitor) override
	{
		visitor.visitUnaryExpr(*this);
	}
};

struct VariableExpr : CreatableType<VariableExpr>, Expr
{
	explicit VariableExpr(Token name)
		: name { std::move(name) }
	{
	}

	Token name;

	void accept(VisitorExpr & visitor) override
	{
		visitor.visitVariableExpr(*this);
	}
};

class BlockStmt;
class ClassStmt;
class ExpressionStmt;
class FunctionStmt;
class IfStmt;
class PrintStmt;
class ReturnStmt;
class VarStmt;
class WhileStmt;

struct VisitorStmt
{
	virtual ~VisitorStmt() = default;

	virtual void visitBlockStmt(BlockStmt & stmt) = 0;
	virtual void visitClassStmt(ClassStmt & stmt) = 0;
	virtual void visitExpressionStmt(ExpressionStmt & stmt) = 0;
	virtual void visitFunctionStmt(FunctionStmt & stmt) = 0;
	virtual void visitIfStmt(IfStmt & stmt) = 0;
	virtual void visitPrintStmt(PrintStmt & stmt) = 0;
	virtual void visitReturnStmt(ReturnStmt & stmt) = 0;
	virtual void visitVarStmt(VarStmt & stmt) = 0;
	virtual void visitWhileStmt(WhileStmt & stmt) = 0;
};

struct Stmt
{
	virtual void accept(VisitorStmt & visitor) = 0;
};

struct BlockStmt : CreatableType<BlockStmt>, Stmt
{
	explicit BlockStmt(std::vector<Stmt*> statements)
		: statements { std::move(statements) }
	{
	}

	std::vector<Stmt*> statements;

	void accept(VisitorStmt & visitor) override
	{
		visitor.visitBlockStmt(*this);
	}
};

struct ClassStmt : CreatableType<ClassStmt>, Stmt
{
	ClassStmt(Token name, std::vector<FunctionStmt*> methods)
		: name { std::move(name) }, methods { std::move(methods) }
	{
	}

	Token name;
	std::vector<FunctionStmt*> methods;

	void accept(VisitorStmt & visitor) override
	{
		visitor.visitClassStmt(*this);
	}
};

struct ExpressionStmt : CreatableType<ExpressionStmt>, Stmt
{
	explicit ExpressionStmt(Expr* expression)
		: expression { expression }
	{
	}

	Expr* expression;

	void accept(VisitorStmt & visitor) override
	{
		visitor.visitExpressionStmt(*this);
	}
};

struct FunctionStmt : CreatableType<FunctionStmt>, Stmt
{
	FunctionStmt(Token name, std::vector<Token> parameters, std::vector<Stmt*> body)
		: name { std::move(name) }, parameters { std::move(parameters) }, body { std::move(body) }
	{
	}

	Token name;
	std::vector<Token> parameters;
	std::vector<Stmt*> body;

	void accept(VisitorStmt & visitor) override
	{
		visitor.visitFunctionStmt(*this);
	}
};

struct IfStmt : CreatableType<IfStmt>, Stmt
{
	IfStmt(Expr* condition, Stmt* thenBranch, Stmt* elseBranch)
		: condition { condition }, thenBranch { thenBranch }, elseBranch { elseBranch }
	{
	}

	Expr* condition;
	Stmt* thenBranch;
	Stmt* elseBranch;

	void accept(VisitorStmt & visitor) override
	{
		visitor.visitIfStmt(*this);
	}
};

struct PrintStmt : CreatableType<PrintStmt>, Stmt
{
	explicit PrintStmt(Expr* expression)
		: expression { expression }
	{
	}

	Expr* expression;

	void accept(VisitorStmt & visitor) override
	{
		visitor.visitPrintStmt(*this);
	}
};

struct ReturnStmt : CreatableType<ReturnStmt>, Stmt
{
	ReturnStmt(Token keyword, Expr* value)
		: keyword { std::move(keyword) }, value { value }
	{
	}

	Token keyword;
	Expr* value;

	void accept(VisitorStmt & visitor) override
	{
		visitor.visitReturnStmt(*this);
	}
};

struct VarStmt : CreatableType<VarStmt>, Stmt
{
	VarStmt(Token name, Expr* initializer)
		: name { std::move(name) }, initializer { initializer }
	{
	}

	Token name;
	Expr* initializer;

	void accept(VisitorStmt & visitor) override
	{
		visitor.visitVarStmt(*this);
	}
};

struct WhileStmt : CreatableType<WhileStmt>, Stmt
{
	WhileStmt(Expr* condition, Stmt* body)
		: condition { condition }, body { body }
	{
	}

	Expr* condition;
	Stmt* body;

	void accept(VisitorStmt & visitor) override
	{
		visitor.visitWhileStmt(*this);
	}
};

#endif //LOXPLUS_AST_H
