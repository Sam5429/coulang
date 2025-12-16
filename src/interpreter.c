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
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <coulang/interpreter.h>
#include <coulang/scope.h>
#include <coulang/macros.h>
#include <coulang/value.h>

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

typedef struct {
	CoulangScope *scope;
	CoulangValue return_value;
} CoulangInterpreterCallFrame;

typedef struct {
	CoulangScope *global_scope;
	CoulangInterpreterCallFrame *call_frames;
	size_t call_frames_len;
	size_t call_frames_capacity;
} CoulangInterpreterStack;

static inline CoulangInterpreterCallFrame
init__CoulangInterpreterCallFrame();

static void
deinit__CoulangInterpreterCallFrame(const CoulangInterpreterCallFrame *const self);

static CoulangInterpreterStack
init__CoulangInterpreterStack();

static void
deinit__CoulangInterpreterStack(const CoulangInterpreterStack *const self);

static CoulangValue
compute_value_from_binary_int_expr__CoulangInterpreter(enum CoulangExprBinaryKind kind, int64_t left, int64_t right);

static CoulangValue
compute_value_from_binary_float_expr__CoulangInterpreter(enum CoulangExprBinaryKind kind, float left, float right);

static CoulangValue
compute_value_from_binary_expr__CoulangInterpreter(const CoulangExpr *expr);

static CoulangValue
compute_value_from_unary_expr__CoulangInterpreter(const CoulangExpr *expr);

static CoulangValue
compute_value_from_list_expr__CoulangInterprerter(const CoulangExpr *expr);

static CoulangValue
compute_value_from_identifier_expr__CoulangInterpreter(const CoulangExpr *expr);

static CoulangValue
compute_value_from_function_call_expr__CoulangInterpreter(const CoulangExpr *expr);

static CoulangValue
compute_value_from_expr__CoulangInterpreter(const CoulangExpr *expr);

static inline void
handle_function_decl__CoulangInterpreter(const CoulangDecl *decl);

static void
handle_load_decl__CoulangInterpreter(const CoulangDecl *decl);

static void
handle_variable_decl__CoulangInterpreter(const CoulangDecl *decl);

static void
handle_decls__CoulangInterpreter(const CoulangDeclFunctionBody *body);

static CoulangInterpreterStack interpreter_stack = {0};

CoulangInterpreterCallFrame
init__CoulangInterpreterCallFrame()
{
	return (CoulangInterpreterCallFrame){
		.scope = init__CoulangScope(),
		.return_value = {0}
	};
}

void
deinit__CoulangInterpreterCallFrame(const CoulangInterpreterCallFrame *const self)
{
	deinit__CoulangScope(self->scope);
}

CoulangInterpreterStack
init__CoulangInterpreterStack()
{
	size_t call_frames_capacity = 8092;
	CoulangInterpreterCallFrame *call_frames = COULANG_ARRAY_ALLOC(call_frames_capacity, sizeof(CoulangInterpreterCallFrame));

	return (CoulangInterpreterStack){
		.global_scope = init__CoulangScope(),
		.call_frames = call_frames,
		.call_frames_len = 0,
		.call_frames_capacity = call_frames_capacity,
	};
}

void
deinit__CoulangInterpreterStack(const CoulangInterpreterStack *const self)
{
	deinit__CoulangScope(self->global_scope);
	free(self->call_frames);
}

#define COMPUTE_VALUE_FROM_BINARY_NUMBER_EXPR(kind, type, left, right) \
	switch (kind) { \
		case COULANG_EXPR_BINARY_KIND_ADD: \
			return init_##type##__CoulangValue(left + right); \
		case COULANG_EXPR_BINARY_KIND_SUB: \
			return init_##type##__CoulangValue(left - right); \
		case COULANG_EXPR_BINARY_KIND_MUL: \
			return init_##type##__CoulangValue(left * right); \
		case COULANG_EXPR_BINARY_KIND_DIV: \
			return init_##type##__CoulangValue(left / right); \
		case COULANG_EXPR_BINARY_KIND_MOD: \
			return init_##type##__CoulangValue((int64_t)left % (int64_t)right); \
		case COULANG_EXPR_BINARY_KIND_EQ: \
			return init_##type##__CoulangValue(left == right); \
		case COULANG_EXPR_BINARY_KIND_NOT_EQ: \
			return init_##type##__CoulangValue(left != right); \
		case COULANG_EXPR_BINARY_KIND_LESS: \
			return init_##type##__CoulangValue(left < right); \
		case COULANG_EXPR_BINARY_KIND_GREATER: \
			return init_##type##__CoulangValue(left > right); \
		case COULANG_EXPR_BINARY_KIND_LESS_EQ: \
			return init_##type##__CoulangValue(left <= right); \
		case COULANG_EXPR_BINARY_KIND_GREATER_EQ: \
			return init_##type##__CoulangValue(left >= right); \
		default: \
			COULANG_UNREACHABLE("unknown binary kind"); \
	}


CoulangValue
compute_value_from_binary_int_expr__CoulangInterpreter(enum CoulangExprBinaryKind kind, int64_t left, int64_t right)
{
	COMPUTE_VALUE_FROM_BINARY_NUMBER_EXPR(kind, int, left, right);
}

CoulangValue
compute_value_from_binary_float_expr__CoulangInterpreter(enum CoulangExprBinaryKind kind, float left, float right)
{
	COMPUTE_VALUE_FROM_BINARY_NUMBER_EXPR(kind, float, left, right);
}

#undef COMPUTE_VALUE_FROM_BINARY_NUMBER_EXPR

CoulangValue
compute_value_from_binary_expr__CoulangInterpreter(const CoulangExpr *expr)
{
	CoulangValue left = compute_value_from_expr__CoulangInterpreter(expr->binary.left);
	CoulangValue right = compute_value_from_expr__CoulangInterpreter(expr->binary.right);
	int value_kind = left.kind;

	value_kind ^= right.kind;

	if (value_kind == 0) {
		CoulangValue res;

		switch (left.kind) {
			case COULANG_VALUE_KIND_INT:
				res = compute_value_from_binary_int_expr__CoulangInterpreter(expr->binary.kind, left.int_, right.int_);

				break;
			case COULANG_VALUE_KIND_FLOAT:
				res = compute_value_from_binary_float_expr__CoulangInterpreter(expr->binary.kind, left.float_, right.float_);

				break;
			default:
				goto error;
		}

		deinit__CoulangValue(&left);
		deinit__CoulangValue(&right);

		return res;
	} else {
error:
		COULANG_INTERPRETER_ERROR("can only handle int v. int, float v. float");
	}
}

CoulangValue
compute_value_from_unary_expr__CoulangInterpreter(const CoulangExpr *expr)
{
	CoulangValue right = compute_value_from_expr__CoulangInterpreter(expr->unary.right);

	if (right.kind == COULANG_VALUE_KIND_INT) {
		CoulangValue res;

		switch (expr->unary.kind) {
			case COULANG_EXPR_UNARY_KIND_NEG:
				res = init_int__CoulangValue(-right.int_);
				
				break;
			case COULANG_EXPR_UNARY_KIND_NOT:
				res = init_int__CoulangValue(!right.int_);

				break;
			default:
				COULANG_UNREACHABLE("unknown unary kind");
		}

		deinit__CoulangValue(&right);

		return res;
	} else {
		COULANG_INTERPRETER_ERROR("can only handle int");
	}
}

CoulangValue
compute_value_from_list_expr__CoulangInterprerter(const CoulangExpr *expr)
{
	size_t len = expr->list.len;
	CoulangValue *buffer = COULANG_ALLOC(sizeof(CoulangValue) * len);
	CoulangExpr *current = expr->list.head;

	for (size_t i = 0; i < len && current; ++i) {
		buffer[i] = compute_value_from_expr__CoulangInterpreter(current);
		current = current->next;
	}

	return init_list__CoulangValue(init__CoulangValueList(buffer, len));
}

CoulangValue
compute_value_from_identifier_expr__CoulangInterpreter(const CoulangExpr *expr)
{
	CoulangScope *current_scope = interpreter_stack.call_frames[interpreter_stack.call_frames_capacity - interpreter_stack.call_frames_len].scope;
	CoulangVariable *variable = get_variable__CoulangScope(current_scope, expr->identifier);

	if (!variable) {
		COULANG_INTERPRETER_ERROR("cannot found variable: %s", variable->name->buffer);
	}

	return variable->value;
}

CoulangValue
compute_value_from_function_call_expr__CoulangInterpreter(const CoulangExpr *expr)
{
}

CoulangValue
compute_value_from_expr__CoulangInterpreter(const CoulangExpr *expr)
{
	switch (expr->kind) {
		case COULANG_EXPR_KIND_BINARY:
			return compute_value_from_binary_expr__CoulangInterpreter(expr);
		case COULANG_EXPR_KIND_UNARY:
			return compute_value_from_unary_expr__CoulangInterpreter(expr);
		case COULANG_EXPR_KIND_STRING:
			return init_str__CoulangValue(expr->string);
		case COULANG_EXPR_KIND_INTEGER:
			return init_int__CoulangValue(expr->integer);
		case COULANG_EXPR_KIND_FLOAT:
			return init_float__CoulangValue(expr->float_);
		case COULANG_EXPR_KIND_LIST:
			return compute_value_from_list_expr__CoulangInterprerter(expr);
		case COULANG_EXPR_KIND_GROUPING:
			return compute_value_from_expr__CoulangInterpreter(expr->grouping);
		case COULANG_EXPR_KIND_IDENTIFIER:
			return compute_value_from_identifier_expr__CoulangInterpreter(expr);
		case COULANG_EXPR_KIND_FUNCTION_CALL:
			break;
		default:
			COULANG_UNREACHABLE("unknown expr kind");
	}
}

void
handle_function_decl__CoulangInterpreter(const CoulangDecl *decl)
{
	add_function__CoulangScope(interpreter_stack.global_scope, init_decl__CoulangFunction(&decl->function));
}

void
handle_load_decl__CoulangInterpreter(const CoulangDecl *decl)
{
	void *lib_handle = dlopen(decl->load.library->buffer, RTLD_LAZY);

	if (!lib_handle) {
		COULANG_INTERPRETER_ERROR("coulang don\'t know how to load this library: %s", decl->load.library->buffer);
	}

	for (size_t i = 0; i < decl->load.symbols.len; ++i) {
		const String *symbol_name = &decl->load.symbols.buffer[i];
		void *symbol = dlsym(lib_handle, symbol_name->buffer);

		if (symbol) {
			add_function__CoulangScope(interpreter_stack.global_scope, init_symbol__CoulangFunction(init__CoulangFunctionSymbol(symbol_name, symbol)));
		} else {
			COULANG_INTERPRETER_ERROR("`%s` symbol cannot be loaded from: %s", symbol_name->buffer, decl->load.library->buffer);
		}
	}

	// TODO: Close this somehow.
	// dlclose(lib);
}

void
handle_variable_decl__CoulangInterpreter(const CoulangDecl *decl)
{
	add_variable__CoulangScope(interpreter_stack.global_scope, init__CoulangVariable(decl->variable.name, compute_value_from_expr__CoulangInterpreter(decl->variable.expr)));
}

void
handle_decls__CoulangInterpreter(const CoulangDeclFunctionBody *body)
{
	for (size_t i = 0; i < body->len; ++i) {
		const CoulangDeclFunctionBodyItem *item = &body->items[i];

		if (item->kind != COULANG_DECL_FUNCTION_BODY_ITEM_KIND_DECL) {
			COULANG_UNREACHABLE("expected to have only declaration item at this point");
		}

		switch (item->decl->kind) {
			case COULANG_DECL_KIND_LOAD:
				handle_load_decl__CoulangInterpreter(item->decl);

				break;
			case COULANG_DECL_KIND_FUNCTION:
				handle_function_decl__CoulangInterpreter(item->decl);

				break;
			case COULANG_DECL_KIND_VARIABLE:
				handle_variable_decl__CoulangInterpreter(item->decl);

				break;
			default:
				COULANG_UNREACHABLE("unknown item kind");
		}
	}
}

void
run__CoulangInterpreter(const CoulangDeclFunctionBody *body)
{
	interpreter_stack = init__CoulangInterpreterStack();

	handle_decls__CoulangInterpreter(body);

	deinit__CoulangInterpreterStack(&interpreter_stack);
}
