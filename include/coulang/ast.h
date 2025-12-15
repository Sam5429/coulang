#ifndef COULANG_AST_H
#define COULANG_AST_H

#include <coulang/string.h>

#include <stdint.h>

typedef struct CoulangExpr CoulangExpr;

enum CoulangExprBinaryKind {
	COULANG_EXPR_BINARY_KIND_ADD,
	COULANG_EXPR_BINARY_KIND_SUB,
	COULANG_EXPR_BINARY_KIND_MUL,
	COULANG_EXPR_BINARY_KIND_DIV,
	COULANG_EXPR_BINARY_KIND_MOD,
	COULANG_EXPR_BINARY_KIND_ASSIGN,
	COULANG_EXPR_BINARY_KIND_EQ,
	COULANG_EXPR_BINARY_KIND_NOT_EQ,
	COULANG_EXPR_BINARY_KIND_LESS,
	COULANG_EXPR_BINARY_KIND_GREATER,
	COULANG_EXPR_BINARY_KIND_LESS_EQ,
	COULANG_EXPR_BINARY_KIND_GREATER_EQ
};

typedef struct {
	enum CoulangExprBinaryKind kind;
	CoulangExpr *left;
	CoulangExpr *right;
} CoulangExprBinary;

static inline CoulangExprBinary init__CoulangExprBinary(enum CoulangExprBinaryKind kind, struct CoulangExpr *left, struct CoulangExpr *right)
{
	return (CoulangExprBinary){
		.kind = kind,
		.left = left,
		.right = right
	};
}

void deinit__CoulangExprBinary(const CoulangExprBinary *const self);

typedef struct {
	struct CoulangExpr *head;
} CoulangExprList;

static inline CoulangExprList init__CoulangExprList(struct CoulangExpr *head)
{
	return (CoulangExprList){
		.head = head
	};
}

void deinit__CoulangExprList(const CoulangExprList *const self);

enum CoulangExprKind {
	COULANG_EXPR_KIND_BINARY,
	COULANG_EXPR_KIND_STRING,
	COULANG_EXPR_KIND_INTEGER,
	COULANG_EXPR_KIND_FLOAT,
	COULANG_EXPR_KIND_LIST,
};

typedef struct CoulangExpr {
	enum CoulangExprKind kind;
	struct CoulangExpr *next;
	union {
		CoulangExprBinary binary;
		const String *string;
		uint64_t integer;
		double float_;
		CoulangExprList list;
	};
} CoulangExpr;

CoulangExpr *
init_binary__CoulangExpr(CoulangExprBinary binary);

CoulangExpr *
init_string__CoulangExpr(const String *string);

CoulangExpr *
init_integer__CoulangExpr(uint64_t integer);

CoulangExpr *
init_float__CoulangExpr(double float_);

CoulangExpr *
init_list__CoulangExpr(CoulangExprList list);

void deinit__CoulangExpr(CoulangExpr *self);

#endif // COULANG_AST_H
