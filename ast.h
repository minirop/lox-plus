#ifndef LOXPLUS_AST_H
#define LOXPLUS_AST_H

#include "Token.h"
#include "Object.h"
#include <vector>
#include <memory>

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

struct AssignExpr : Expr
{
	AssignExpr(Token name, std::unique_ptr<Expr> value)
		: name { std::move(name) }, value { std::move(value) }
	{
	}

	Token name;
	std::unique_ptr<Expr> value;

	void accept(VisitorExpr & visitor) override
	{
		visitor.visitAssignExpr(*this);
	}
};

struct BinaryExpr : Expr
{
	BinaryExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
		: left { std::move(left) }, op { std::move(op) }, right { std::move(right) }
	{
	}

	std::unique_ptr<Expr> left;
	Token op;
	std::unique_ptr<Expr> right;

	void accept(VisitorExpr & visitor) override
	{
		visitor.visitBinaryExpr(*this);
	}
};

struct CallExpr : Expr
{
	CallExpr(std::unique_ptr<Expr> callee, Token paren, std::vector<std::unique_ptr<Expr>> arguments)
		: callee { std::move(callee) }, paren { std::move(paren) }, arguments { std::move(arguments) }
	{
	}

	std::unique_ptr<Expr> callee;
	Token paren;
	std::vector<std::unique_ptr<Expr>> arguments;

	void accept(VisitorExpr & visitor) override
	{
		visitor.visitCallExpr(*this);
	}
};

struct GetExpr : Expr
{
	GetExpr(std::unique_ptr<Expr> object, Token name)
		: object { std::move(object) }, name { std::move(name) }
	{
	}

	std::unique_ptr<Expr> object;
	Token name;

	void accept(VisitorExpr & visitor) override
	{
		visitor.visitGetExpr(*this);
	}
};

struct GroupingExpr : Expr
{
	explicit GroupingExpr(std::unique_ptr<Expr> expression)
		: expression { std::move(expression) }
	{
	}

	std::unique_ptr<Expr> expression;

	void accept(VisitorExpr & visitor) override
	{
		visitor.visitGroupingExpr(*this);
	}
};

struct LiteralExpr : Expr
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

struct LogicalExpr : Expr
{
	LogicalExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
		: left { std::move(left) }, op { std::move(op) }, right { std::move(right) }
	{
	}

	std::unique_ptr<Expr> left;
	Token op;
	std::unique_ptr<Expr> right;

	void accept(VisitorExpr & visitor) override
	{
		visitor.visitLogicalExpr(*this);
	}
};

struct SetExpr : Expr
{
	SetExpr(std::unique_ptr<Expr> object, Token name, std::unique_ptr<Expr> value)
		: object { std::move(object) }, name { std::move(name) }, value { std::move(value) }
	{
	}

	std::unique_ptr<Expr> object;
	Token name;
	std::unique_ptr<Expr> value;

	void accept(VisitorExpr & visitor) override
	{
		visitor.visitSetExpr(*this);
	}
};

struct ThisExpr : Expr
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

struct UnaryExpr : Expr
{
	UnaryExpr(Token op, std::unique_ptr<Expr> right)
		: op { std::move(op) }, right { std::move(right) }
	{
	}

	Token op;
	std::unique_ptr<Expr> right;

	void accept(VisitorExpr & visitor) override
	{
		visitor.visitUnaryExpr(*this);
	}
};

struct VariableExpr : Expr
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

struct BlockStmt : Stmt
{
	explicit BlockStmt(std::vector<std::unique_ptr<Stmt>> statements)
		: statements { std::move(statements) }
	{
	}

	std::vector<std::unique_ptr<Stmt>> statements;

	void accept(VisitorStmt & visitor) override
	{
		visitor.visitBlockStmt(*this);
	}
};

struct ClassStmt : Stmt
{
	ClassStmt(Token name, std::vector<std::unique_ptr<FunctionStmt>> methods)
		: name { std::move(name) }, methods { std::move(methods) }
	{
	}

	Token name;
	std::vector<std::unique_ptr<FunctionStmt>> methods;

	void accept(VisitorStmt & visitor) override
	{
		visitor.visitClassStmt(*this);
	}
};

struct ExpressionStmt : Stmt
{
	explicit ExpressionStmt(std::unique_ptr<Expr> expression)
		: expression { std::move(expression) }
	{
	}

	std::unique_ptr<Expr> expression;

	void accept(VisitorStmt & visitor) override
	{
		visitor.visitExpressionStmt(*this);
	}
};

struct FunctionStmt : Stmt
{
	FunctionStmt(Token name, std::vector<Token> parameters, std::vector<std::unique_ptr<Stmt>> body)
		: name { std::move(name) }, parameters { std::move(parameters) }, body { std::move(body) }
	{
	}

	Token name;
	std::vector<Token> parameters;
	std::vector<std::unique_ptr<Stmt>> body;

	void accept(VisitorStmt & visitor) override
	{
		visitor.visitFunctionStmt(*this);
	}
};

struct IfStmt : Stmt
{
	IfStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch)
		: condition { std::move(condition) }, thenBranch { std::move(thenBranch) }, elseBranch { std::move(elseBranch) }
	{
	}

	std::unique_ptr<Expr> condition;
	std::unique_ptr<Stmt> thenBranch;
	std::unique_ptr<Stmt> elseBranch;

	void accept(VisitorStmt & visitor) override
	{
		visitor.visitIfStmt(*this);
	}
};

struct PrintStmt : Stmt
{
	explicit PrintStmt(std::unique_ptr<Expr> expression)
		: expression { std::move(expression) }
	{
	}

	std::unique_ptr<Expr> expression;

	void accept(VisitorStmt & visitor) override
	{
		visitor.visitPrintStmt(*this);
	}
};

struct ReturnStmt : Stmt
{
	ReturnStmt(Token keyword, std::unique_ptr<Expr> value)
		: keyword { std::move(keyword) }, value { std::move(value) }
	{
	}

	Token keyword;
	std::unique_ptr<Expr> value;

	void accept(VisitorStmt & visitor) override
	{
		visitor.visitReturnStmt(*this);
	}
};

struct VarStmt : Stmt
{
	VarStmt(Token name, std::unique_ptr<Expr> initializer)
		: name { std::move(name) }, initializer { std::move(initializer) }
	{
	}

	Token name;
	std::unique_ptr<Expr> initializer;

	void accept(VisitorStmt & visitor) override
	{
		visitor.visitVarStmt(*this);
	}
};

struct WhileStmt : Stmt
{
	WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
		: condition { std::move(condition) }, body { std::move(body) }
	{
	}

	std::unique_ptr<Expr> condition;
	std::unique_ptr<Stmt> body;

	void accept(VisitorStmt & visitor) override
	{
		visitor.visitWhileStmt(*this);
	}
};

#endif //LOXPLUS_AST_H
