// MIT License
// 
// Copyright (c) 2025 moi_c_sam, ArthurPV
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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
