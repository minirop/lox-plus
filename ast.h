#ifndef LOXPLUS_AST_H
#define LOXPLUS_AST_H

#include "Token.h"
#include "Object.h"
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

struct AssignExpr : Expr
{
	AssignExpr(Token name, Expr* value)
		: name { name }, value { value }
	{
	}

	Token name;
	Expr* value;

	void accept(VisitorExpr & visitor)
	{
		visitor.visitAssignExpr(*this);
	}
};

struct BinaryExpr : Expr
{
	BinaryExpr(Expr* left, Token op, Expr* right)
		: left { left }, op { op }, right { right }
	{
	}

	Expr* left;
	Token op;
	Expr* right;

	void accept(VisitorExpr & visitor)
	{
		visitor.visitBinaryExpr(*this);
	}
};

struct CallExpr : Expr
{
	CallExpr(Expr* callee, Token paren, std::vector<Expr*> arguments)
		: callee { callee }, paren { paren }, arguments { arguments }
	{
	}

	Expr* callee;
	Token paren;
	std::vector<Expr*> arguments;

	void accept(VisitorExpr & visitor)
	{
		visitor.visitCallExpr(*this);
	}
};

struct GetExpr : Expr
{
	GetExpr(Expr* object, Token name)
		: object { object }, name { name }
	{
	}

	Expr* object;
	Token name;

	void accept(VisitorExpr & visitor)
	{
		visitor.visitGetExpr(*this);
	}
};

struct GroupingExpr : Expr
{
	explicit GroupingExpr(Expr* expression)
		: expression { expression }
	{
	}

	Expr* expression;

	void accept(VisitorExpr & visitor)
	{
		visitor.visitGroupingExpr(*this);
	}
};

struct LiteralExpr : Expr
{
	explicit LiteralExpr(Object value)
		: value { value }
	{
	}

	Object value;

	void accept(VisitorExpr & visitor)
	{
		visitor.visitLiteralExpr(*this);
	}
};

struct LogicalExpr : Expr
{
	LogicalExpr(Expr* left, Token op, Expr* right)
		: left { left }, op { op }, right { right }
	{
	}

	Expr* left;
	Token op;
	Expr* right;

	void accept(VisitorExpr & visitor)
	{
		visitor.visitLogicalExpr(*this);
	}
};

struct SetExpr : Expr
{
	SetExpr(Expr* object, Token name, Expr* value)
		: object { object }, name { name }, value { value }
	{
	}

	Expr* object;
	Token name;
	Expr* value;

	void accept(VisitorExpr & visitor)
	{
		visitor.visitSetExpr(*this);
	}
};

struct ThisExpr : Expr
{
	explicit ThisExpr(Token keyword)
		: keyword { keyword }
	{
	}

	Token keyword;

	void accept(VisitorExpr & visitor)
	{
		visitor.visitThisExpr(*this);
	}
};

struct UnaryExpr : Expr
{
	UnaryExpr(Token op, Expr* right)
		: op { op }, right { right }
	{
	}

	Token op;
	Expr* right;

	void accept(VisitorExpr & visitor)
	{
		visitor.visitUnaryExpr(*this);
	}
};

struct VariableExpr : Expr
{
	explicit VariableExpr(Token name)
		: name { name }
	{
	}

	Token name;

	void accept(VisitorExpr & visitor)
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
	explicit BlockStmt(std::vector<Stmt*> statements)
		: statements { statements }
	{
	}

	std::vector<Stmt*> statements;

	void accept(VisitorStmt & visitor)
	{
		visitor.visitBlockStmt(*this);
	}
};

struct ClassStmt : Stmt
{
	ClassStmt(Token name, std::vector<FunctionStmt*> methods)
		: name { name }, methods { methods }
	{
	}

	Token name;
	std::vector<FunctionStmt*> methods;

	void accept(VisitorStmt & visitor)
	{
		visitor.visitClassStmt(*this);
	}
};

struct ExpressionStmt : Stmt
{
	explicit ExpressionStmt(Expr* expression)
		: expression { expression }
	{
	}

	Expr* expression;

	void accept(VisitorStmt & visitor)
	{
		visitor.visitExpressionStmt(*this);
	}
};

struct FunctionStmt : Stmt
{
	FunctionStmt(Token name, std::vector<Token> parameters, std::vector<Stmt*> body)
		: name { name }, parameters { parameters }, body { body }
	{
	}

	Token name;
	std::vector<Token> parameters;
	std::vector<Stmt*> body;

	void accept(VisitorStmt & visitor)
	{
		visitor.visitFunctionStmt(*this);
	}
};

struct IfStmt : Stmt
{
	IfStmt(Expr* condition, Stmt* thenBranch, Stmt* elseBranch)
		: condition { condition }, thenBranch { thenBranch }, elseBranch { elseBranch }
	{
	}

	Expr* condition;
	Stmt* thenBranch;
	Stmt* elseBranch;

	void accept(VisitorStmt & visitor)
	{
		visitor.visitIfStmt(*this);
	}
};

struct PrintStmt : Stmt
{
	explicit PrintStmt(Expr* expression)
		: expression { expression }
	{
	}

	Expr* expression;

	void accept(VisitorStmt & visitor)
	{
		visitor.visitPrintStmt(*this);
	}
};

struct ReturnStmt : Stmt
{
	ReturnStmt(Token keyword, Expr* value)
		: keyword { keyword }, value { value }
	{
	}

	Token keyword;
	Expr* value;

	void accept(VisitorStmt & visitor)
	{
		visitor.visitReturnStmt(*this);
	}
};

struct VarStmt : Stmt
{
	VarStmt(Token name, Expr* initializer)
		: name { name }, initializer { initializer }
	{
	}

	Token name;
	Expr* initializer;

	void accept(VisitorStmt & visitor)
	{
		visitor.visitVarStmt(*this);
	}
};

struct WhileStmt : Stmt
{
	WhileStmt(Expr* condition, Stmt* body)
		: condition { condition }, body { body }
	{
	}

	Expr* condition;
	Stmt* body;

	void accept(VisitorStmt & visitor)
	{
		visitor.visitWhileStmt(*this);
	}
};

#endif //LOXPLUS_AST_H
