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

void deinit__CoulangExprUnary(const CoulangExprUnary *const self)
{
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

CoulangExpr *init_unary__CoulangExpr(CoulangExprUnary unary)
{
	INIT_EXPR(COULANG_EXPR_KIND_UNARY, .unary = unary);
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

CoulangExpr *
init_grouping__CoulangExpr(CoulangExpr *grouping)
{
	INIT_EXPR(COULANG_EXPR_KIND_GROUPING, .grouping = grouping);
}

CoulangExpr *
init_identifier__CoulangExpr(const String *identifier)
{
	INIT_EXPR(COULANG_EXPR_KIND_IDENTIFIER, .identifier = identifier);
}

#undef INIT_EXPR

void deinit__CoulangExpr(CoulangExpr *self)
{
	switch (self->kind) {
		case COULANG_EXPR_KIND_BINARY:
			deinit__CoulangExprBinary(&self->binary);

			break;
		case COULANG_EXPR_KIND_UNARY:
			deinit__CoulangExprUnary(&self->unary);

			break;
		case COULANG_EXPR_KIND_LIST:
			deinit__CoulangExprList(&self->list);

			break;
		case COULANG_EXPR_KIND_GROUPING:
			deinit__CoulangExpr(self->grouping);

			break;
		default:
			break;
	}

	free(self);
}

void
deinit__CoulangStmtIfBranch(CoulangStmtIfBranch *self)
{
	CoulangStmtIfBranch *current = self;

	do {
		CoulangStmtIfBranch *next = current->next;

		deinit__CoulangExpr(current->cond);
		deinit__CoulangDeclFunctionBody(current->body);
		free(current->body);
		free(current);

		current = next;
	} while (current);
}

void deinit__CoulangStmtIf(const CoulangStmtIf *const self)
{
	deinit__CoulangStmtIfBranch(self->ifs);

	if (self->else_) {
		deinit__CoulangDeclFunctionBody(self->else_);
		free(self->else_);
	}
}

void deinit__CoulangStmtWhile(const CoulangStmtWhile *const self)
{
	deinit__CoulangExpr(self->cond);
	deinit__CoulangDeclFunctionBody(self->body);
	free(self->body);
}

void
deinit__CoulangStmt(const CoulangStmt *const self)
{
	switch (self->kind) {
		case COULANG_STMT_KIND_IF:
			deinit__CoulangStmtIf(&self->if_);

			break;
		case COULANG_STMT_KIND_RETURN:
			deinit__CoulangExpr(self->return_);

			break;
		case COULANG_STMT_KIND_WHILE:
			deinit__CoulangStmtWhile(&self->while_);

			break;
		default:
			COULANG_UNREACHABLE("unknown function stmt item kind");
	}
}

CoulangDeclFunctionParam *
init__CoulangDeclFunctionParam(const String *name, enum CoulangDataType data_type)
{
	CoulangDeclFunctionParam *self = COULANG_ALLOC(sizeof(CoulangDeclFunctionParam));

	*self = (CoulangDeclFunctionParam){
		.name = name,
		.data_type = data_type,
		.next = NULL,
	};

	return self;
}

void
deinit__CoulangDeclFunctionParam(CoulangDeclFunctionParam *self)
{
	CoulangDeclFunctionParam *current = self;

	while (current) {
		CoulangDeclFunctionParam *next = current->next;

		free(current);

		current = next;
	}
}

void
deinit__CoulangDeclFunctionBodyItem(CoulangDeclFunctionBodyItem *self)
{
	switch (self->kind) {
		case COULANG_DECL_FUNCTION_BODY_ITEM_KIND_EXPR:
			deinit__CoulangExpr(self->expr);

			break;
		case COULANG_DECL_FUNCTION_BODY_ITEM_KIND_STMT:
			deinit__CoulangStmt(&self->stmt);

			break;
		case COULANG_DECL_FUNCTION_BODY_ITEM_KIND_DECL:
			deinit__CoulangDecl(self->decl);
			free(self->decl);

			break;
		default:
			COULANG_UNREACHABLE("unknown function body item kind");
	}
}

void
add__CoulangDeclFunctionBody(CoulangDeclFunctionBody *self, CoulangDeclFunctionBodyItem item)
{
	if (!self->items) {
		self->items = COULANG_ALLOC(self->capacity * sizeof(CoulangDeclFunctionBodyItem));
	} else if (self->capacity == self->len) {
		self->capacity *= 2;
		self->items = COULANG_REALLOC(self->items, self->capacity * sizeof(CoulangDeclFunctionBodyItem));
	}

	self->items[self->len++] = item;
}

void
deinit__CoulangDeclFunctionBody(const CoulangDeclFunctionBody *const self)
{
	for (size_t i = 0; i < self->len; ++i) {
		deinit__CoulangDeclFunctionBodyItem(&self->items[i]);
	}

	free(self->items);
}

void deinit__CoulangDecl(const CoulangDecl *const self)
{
	switch (self->kind) {
		case COULANG_DECL_KIND_LOAD:
			deinit__CoulangDeclLoad(&self->load);

			break;
		case COULANG_DECL_KIND_FUNCTION:
			deinit__CoulangDeclFunction(&self->function);

			break;
		case COULANG_DECL_KIND_VARIABLE:
			deinit__CoulangDeclVariable(&self->variable);

			break;
		default:
			COULANG_UNREACHABLE("unknown declaration kind");
	}
}
