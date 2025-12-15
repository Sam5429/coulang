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

#include <stdlib.h>
#include <stdio.h>

#include <coulang/ast.h>
#include <coulang/macros.h>

void deinit__CoulangExprBinary(const CoulangExprBinary *const self)
{
	deinit__CoulangExpr(self->left);
	deinit__CoulangExpr(self->right);
}

void deinit__CoulangExprList(const CoulangExprList *const self)
{
	CoulangExpr *current = self->head;

	while (current) {
		CoulangExpr *next = current->next;

		deinit__CoulangExpr(current);

		current = next;
	}
}

#define INIT_EXPR(k, ...) \
	CoulangExpr *expr = COULANG_ALLOC(sizeof(CoulangExpr)); \
	*expr = (CoulangExpr){ .kind = (k), __VA_ARGS__ }; \
	return expr;

CoulangExpr *
init_binary__CoulangExpr(CoulangExprBinary binary)
{
	INIT_EXPR(COULANG_EXPR_KIND_BINARY, .binary = binary);
}

CoulangExpr *
init_string__CoulangExpr(const String *string)
{
	INIT_EXPR(COULANG_EXPR_KIND_STRING, .string = string);
}

CoulangExpr *
init_integer__CoulangExpr(uint64_t integer)
{
	INIT_EXPR(COULANG_EXPR_KIND_INTEGER, .integer = integer);
}

CoulangExpr *
init_float__CoulangExpr(double float_)
{
	INIT_EXPR(COULANG_EXPR_KIND_FLOAT, .float_ = float_);
}

CoulangExpr *
init_list__CoulangExpr(CoulangExprList list)
{
	INIT_EXPR(COULANG_EXPR_KIND_LIST, .list = list);
}

#undef INIT_EXPR

void deinit__CoulangExpr(CoulangExpr *self)
{
	switch (self->kind) {
		case COULANG_EXPR_KIND_BINARY:
			deinit__CoulangExprBinary(&self->binary);

			break;
		case COULANG_EXPR_KIND_LIST:
			deinit__CoulangExprList(&self->list);

			break;
		default:
			break;
	}

	free(self);
}
