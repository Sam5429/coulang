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
