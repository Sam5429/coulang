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
#include <coulang/macros.h>
#include <coulang/scope.h>
#include <coulang/value.h>
#include <coulang/symbol_runner.h>

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

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

static inline CoulangInterpreterCallFrame init__CoulangInterpreterCallFrame();

static void deinit__CoulangInterpreterCallFrame(
    const CoulangInterpreterCallFrame *const self);

static CoulangInterpreterStack init__CoulangInterpreterStack();

static void
deinit__CoulangInterpreterStack(const CoulangInterpreterStack *const self);

static CoulangValue compute_value_from_binary_int_expr__CoulangInterpreter(
    enum CoulangExprBinaryKind kind, int64_t left, int64_t right);

static CoulangValue compute_value_from_binary_float_expr__CoulangInterpreter(
    enum CoulangExprBinaryKind kind, float left, float right);

static CoulangValue
compute_value_from_binary_expr__CoulangInterpreter(const CoulangExpr *expr);

static CoulangValue
compute_value_from_unary_expr__CoulangInterpreter(const CoulangExpr *expr);

static CoulangValue
compute_value_from_list_expr__CoulangInterprerter(const CoulangExpr *expr);

static CoulangValue
compute_value_from_identifier_expr__CoulangInterpreter(const CoulangExpr *expr);

static CoulangValue compute_value_from_function_symbol_call_expr__CoulangInterpreter(
	const CoulangExpr *expr,
    CoulangFunction *function);

static CoulangValue
compute_value_from_function_decl_call_expr__CoulangInterpreter(
	const CoulangExpr *expr,
    CoulangFunction *function);

static CoulangValue compute_value_from_function_call_expr__CoulangInterpreter(
    const CoulangExpr *expr);

static CoulangValue
compute_value_from_expr__CoulangInterpreter(const CoulangExpr *expr);

static inline void
handle_function_decl__CoulangInterpreter(const CoulangDecl *decl);

static void handle_load_decl__CoulangInterpreter(const CoulangDecl *decl);

static void handle_variable_decl__CoulangInterpreter(const CoulangDecl *decl);

static void
handle_decls__CoulangInterpreter(const CoulangDeclFunctionBody *body);

static void
compute_if_stmt__CoulangInterpreter(const CoulangStmt *stmt);

static void
compute_return_stmt__CoulangInterpreter(const CoulangStmt *stmt);

static void
compute_while_stmt__CoulangInterpreter(const CoulangStmt *stmt);

static void
coulang_stmt__CoulangInterpreter(const CoulangStmt *stmt);

static void
push_call_frame__CoulangInterpreter(CoulangExpr *called_params, CoulangDeclFunctionParam *decl_params);

static CoulangInterpreterCallFrame
pop_call_frame__CoulangInterpreter();

static void
add_current_scope__CoulangInterpreter();

static void
remove_current_scope__CoulangInterpreter();

static void
compute_body__CoulangInterpreter(const CoulangDeclFunctionBody *body);

static void
compute_child_body__CoulangInterpreter(const CoulangDeclFunctionBody *body);

static void
compute_function_body__CoulangInterpreter(CoulangExpr *called_params, CoulangDeclFunctionParam *decl_params, const CoulangDeclFunctionBody *body);

#define CURRENT_FRAME() (interpreter_stack.call_frames_len > 0 ? \
      &interpreter_stack \
          .call_frames[interpreter_stack.call_frames_len - 1]: \
			NULL)

#define CURRENT_SCOPE_REF() ({ \
	CoulangInterpreterCallFrame *_frame = CURRENT_FRAME(); \
	CoulangScope **_scope = &interpreter_stack.global_scope; \
	\
	if (_frame) { \
		_scope = &_frame->scope; \
	} \
	\
	_scope; \
})

#define CURRENT_SCOPE() (*CURRENT_SCOPE_REF())

static CoulangInterpreterStack interpreter_stack = {0};

CoulangInterpreterCallFrame init__CoulangInterpreterCallFrame() {
  CoulangScope *scope = init__CoulangScope();

  scope->parent = interpreter_stack.global_scope;

  return (CoulangInterpreterCallFrame){.scope = scope,
                                       .return_value = {0}};
}

void deinit__CoulangInterpreterCallFrame(
    const CoulangInterpreterCallFrame *const self) {
  deinit__CoulangScope(self->scope);
}

CoulangInterpreterStack init__CoulangInterpreterStack() {
  size_t call_frames_capacity = 8092;
  CoulangInterpreterCallFrame *call_frames = COULANG_ARRAY_ALLOC(
      call_frames_capacity, sizeof(CoulangInterpreterCallFrame));

  return (CoulangInterpreterStack){
      .global_scope = init__CoulangScope(),
      .call_frames = call_frames,
      .call_frames_len = 0,
      .call_frames_capacity = call_frames_capacity,
  };
}

void deinit__CoulangInterpreterStack(
    const CoulangInterpreterStack *const self) {
  deinit__CoulangScope(self->global_scope);
  free(self->call_frames);
}

#define COMPUTE_VALUE_FROM_BINARY_NUMBER_EXPR(kind, type, left, right)         \
  switch (kind) {                                                              \
  case COULANG_EXPR_BINARY_KIND_ADD:                                           \
    return init_##type##__CoulangValue(left + right);                          \
  case COULANG_EXPR_BINARY_KIND_SUB:                                           \
    return init_##type##__CoulangValue(left - right);                          \
  case COULANG_EXPR_BINARY_KIND_MUL:                                           \
    return init_##type##__CoulangValue(left * right);                          \
  case COULANG_EXPR_BINARY_KIND_DIV:                                           \
    return init_##type##__CoulangValue(left / right);                          \
  case COULANG_EXPR_BINARY_KIND_MOD:                                           \
    return init_##type##__CoulangValue((int64_t)left % (int64_t)right);        \
  case COULANG_EXPR_BINARY_KIND_EQ:                                            \
    return init_##type##__CoulangValue(left == right);                         \
  case COULANG_EXPR_BINARY_KIND_NOT_EQ:                                        \
    return init_##type##__CoulangValue(left != right);                         \
  case COULANG_EXPR_BINARY_KIND_LESS:                                          \
    return init_##type##__CoulangValue(left < right);                          \
  case COULANG_EXPR_BINARY_KIND_GREATER:                                       \
    return init_##type##__CoulangValue(left > right);                          \
  case COULANG_EXPR_BINARY_KIND_LESS_EQ:                                       \
    return init_##type##__CoulangValue(left <= right);                         \
  case COULANG_EXPR_BINARY_KIND_GREATER_EQ:                                    \
    return init_##type##__CoulangValue(left >= right);                         \
  default:                                                                     \
    COULANG_UNREACHABLE("unknown binary kind");                                \
  }

CoulangValue compute_value_from_binary_int_expr__CoulangInterpreter(
    enum CoulangExprBinaryKind kind, int64_t left, int64_t right) {
  COMPUTE_VALUE_FROM_BINARY_NUMBER_EXPR(kind, int, left, right);
}

CoulangValue compute_value_from_binary_float_expr__CoulangInterpreter(
    enum CoulangExprBinaryKind kind, float left, float right) {
  COMPUTE_VALUE_FROM_BINARY_NUMBER_EXPR(kind, float, left, right);
}

#undef COMPUTE_VALUE_FROM_BINARY_NUMBER_EXPR

CoulangValue
compute_value_from_binary_expr__CoulangInterpreter(const CoulangExpr *expr) {
  CoulangValue left =
      compute_value_from_expr__CoulangInterpreter(expr->binary.left);
  CoulangValue right =
      compute_value_from_expr__CoulangInterpreter(expr->binary.right);

  if ((left.kind ^ right.kind) == 0) {
    CoulangValue res;

    switch (left.kind) {
    case COULANG_VALUE_KIND_INT:
      res = compute_value_from_binary_int_expr__CoulangInterpreter(
          expr->binary.kind, left.int_, right.int_);

      break;
    case COULANG_VALUE_KIND_FLOAT:
      res = compute_value_from_binary_float_expr__CoulangInterpreter(
          expr->binary.kind, left.float_, right.float_);

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
compute_value_from_unary_expr__CoulangInterpreter(const CoulangExpr *expr) {
  CoulangValue right =
      compute_value_from_expr__CoulangInterpreter(expr->unary.right);

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
compute_value_from_list_expr__CoulangInterprerter(const CoulangExpr *expr) {
  size_t len = expr->list.len;
  CoulangValue *buffer = COULANG_ALLOC(sizeof(CoulangValue) * len);
  CoulangExpr *current = expr->list.head;

  for (size_t i = 0; i < len && current; ++i) {
    buffer[i] = compute_value_from_expr__CoulangInterpreter(current);
    current = current->next;
  }

  return init_list__CoulangValue(init__CoulangValueList(buffer, len));
}

CoulangValue compute_value_from_identifier_expr__CoulangInterpreter(
    const CoulangExpr *expr) {
  CoulangScope *current_scope = CURRENT_SCOPE();
  CoulangVariable *variable =
      get_variable__CoulangScope(current_scope, expr->identifier);

  if (!variable) {
    COULANG_INTERPRETER_ERROR("cannot found variable: %s",
                              expr->identifier->buffer);
  }

  return variable->value;
}

CoulangValue compute_value_from_function_symbol_call_expr__CoulangInterpreter(
	const CoulangExpr *expr,
    CoulangFunction *function) {
	size_t params_len = expr->function_call.params_len;
	CoulangExpr *current = expr->function_call.params;
	CoulangValue *params = COULANG_ALLOC(sizeof(CoulangValue) * params_len);

	for (size_t i = 0; i < params_len && current; ++i) {
		params[i] = compute_value_from_expr__CoulangInterpreter(current);
		current = current->next;
	}

	CoulangValue return_value = run__SymbolRunner(function->symbol.addr, params, params_len, function->symbol.data_type);

	for (size_t i = 0; i < params_len; ++i) {
		deinit__CoulangValue(&params[i]);
	}

	free(params);

	return return_value;
}

CoulangValue compute_value_from_function_decl_call_expr__CoulangInterpreter(
	const CoulangExpr *expr,
    CoulangFunction *function) {
	// FIXME: Need to figure out a way, to be able to resume the
	// execution of a function, after the return of a function
	// call, by avoiding to call
	// `compute_function_body__CoulangInterpreter`
	// recursively.
	compute_function_body__CoulangInterpreter(expr->function_call.params, function->decl->params, &function->decl->body);

	CoulangInterpreterCallFrame popped_frame = pop_call_frame__CoulangInterpreter();
	CoulangValue return_value = popped_frame.return_value;

	deinit__CoulangInterpreterCallFrame(&popped_frame);

	return return_value;
}

CoulangValue compute_value_from_function_call_expr__CoulangInterpreter(
    const CoulangExpr *expr) {
  // NOTE: For the time being, we can just have function defined
  // on the global scope.
  CoulangScope *global_scope = interpreter_stack.global_scope;
  CoulangFunction *function =
      get_function__CoulangScope(global_scope, expr->function_call.name);

  if (!function) {
    COULANG_INTERPRETER_ERROR("cannot found function: %s",
                              expr->function_call.name->buffer);
  }

  switch (function->kind) {
  case COULANG_FUNCTION_KIND_SYMBOL:
    return compute_value_from_function_symbol_call_expr__CoulangInterpreter(
		expr,
        function);
  case COULANG_FUNCTION_KIND_DECL:
    return compute_value_from_function_decl_call_expr__CoulangInterpreter(
		expr,
        function);
  default:
    COULANG_UNREACHABLE("unknown function kind");
  }
}

CoulangValue
compute_value_from_expr__CoulangInterpreter(const CoulangExpr *expr) {
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
    return compute_value_from_function_call_expr__CoulangInterpreter(expr);
  default:
    COULANG_UNREACHABLE("unknown expr kind");
  }
}

void handle_function_decl__CoulangInterpreter(const CoulangDecl *decl) {
  add_function__CoulangScope(interpreter_stack.global_scope,
                             init_decl__CoulangFunction(&decl->function));
}

void handle_load_decl__CoulangInterpreter(const CoulangDecl *decl) {
  void *lib_handle = dlopen(decl->load.library->buffer, RTLD_LAZY);

  if (!lib_handle) {
    COULANG_INTERPRETER_ERROR(
        "coulang don\'t know how to load this library: %s",
        decl->load.library->buffer);
  }

  CoulangDeclLoadFunction *current = decl->load.symbols;
  while (current) {
    const String *symbol_name = current->name;
    void *addr = dlsym(lib_handle, symbol_name->buffer);

    if (addr) {
      add_function__CoulangScope(
          interpreter_stack.global_scope,
          init_symbol__CoulangFunction(
              init__CoulangFunctionSymbol(symbol_name, addr, current->data_type)));
    } else {
      COULANG_INTERPRETER_ERROR("`%s` symbol cannot be loaded from: %s",
                                symbol_name->buffer,
                                decl->load.library->buffer);
    }
    current = current->next;
  }

  // TODO: Close this somehow.
  // dlclose(lib);
}

void handle_variable_decl__CoulangInterpreter(const CoulangDecl *decl) {
  CoulangScope *current_scope = CURRENT_SCOPE();

  add_variable__CoulangScope(
      current_scope,
      init__CoulangVariable(
          decl->variable.name,
          compute_value_from_expr__CoulangInterpreter(decl->variable.expr)));
}

void handle_decls__CoulangInterpreter(const CoulangDeclFunctionBody *body) {
  for (size_t i = 0; i < body->len; ++i) {
    const CoulangDeclFunctionBodyItem *item = &body->items[i];

    if (item->kind != COULANG_DECL_FUNCTION_BODY_ITEM_KIND_DECL) {
      COULANG_UNREACHABLE(
          "expected to have only declaration item at this point");
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
compute_if_stmt__CoulangInterpreter(const CoulangStmt *stmt)
{
	CoulangStmtIfBranch *if_branch = stmt->if_.ifs;

	while (if_branch) {
		CoulangValue value = compute_value_from_expr__CoulangInterpreter(if_branch->cond);

		if (is_cond_true__CoulangValue(&value)) {
			deinit__CoulangValue(&value);
			compute_child_body__CoulangInterpreter(if_branch->body);

			return;
		}

		deinit__CoulangValue(&value);

		if_branch = if_branch->next;
	}

	if (stmt->if_.else_) {
		compute_child_body__CoulangInterpreter(stmt->if_.else_);
	}
}

void
compute_return_stmt__CoulangInterpreter(const CoulangStmt *stmt)
{
	CoulangValue return_value = compute_value_from_expr__CoulangInterpreter(stmt->return_);
	CoulangInterpreterCallFrame *current_frame = CURRENT_FRAME();

	if (current_frame) {
		current_frame->return_value = return_value;

		return;
	}

    COULANG_UNREACHABLE("return is impossible here");
}

void
compute_while_stmt__CoulangInterpreter(const CoulangStmt *stmt)
{
	while (true) {
		CoulangValue value = compute_value_from_expr__CoulangInterpreter(stmt->while_.cond);

		if (is_cond_true__CoulangValue(&value)) {
			deinit__CoulangValue(&value);
			compute_child_body__CoulangInterpreter(stmt->while_.body);

			continue;
		}

		deinit__CoulangValue(&value);

		break;
	}
}

void
coulang_stmt__CoulangInterpreter(const CoulangStmt *stmt)
{
	switch (stmt->kind) {
		case COULANG_STMT_KIND_IF:
			return compute_if_stmt__CoulangInterpreter(stmt);
		case COULANG_STMT_KIND_RETURN:
			return compute_return_stmt__CoulangInterpreter(stmt);
		case COULANG_STMT_KIND_WHILE:
			return compute_while_stmt__CoulangInterpreter(stmt);
		default:
			COULANG_UNREACHABLE("unknown statement kind");
	}
}

void
push_call_frame__CoulangInterpreter(CoulangExpr *called_params, CoulangDeclFunctionParam *decl_params)
{
	if (interpreter_stack.call_frames_len < interpreter_stack.call_frames_capacity) {
		interpreter_stack.call_frames[interpreter_stack.call_frames_len++] = init__CoulangInterpreterCallFrame();
		CoulangScope *current_scope = CURRENT_SCOPE();
		CoulangExpr *current_called_param = called_params;
		CoulangDeclFunctionParam *current_decl_param = decl_params;

		while (current_called_param && current_decl_param) {
			CoulangValue param_value = compute_value_from_expr__CoulangInterpreter(current_called_param);

			add_variable__CoulangScope(current_scope, init__CoulangVariable(current_decl_param->name, param_value));

			current_called_param = current_called_param->next;
			current_decl_param = current_decl_param->next;
		}

		if (current_called_param || current_decl_param) {
			COULANG_INTERPRETER_ERROR("missing parameter");
		}
	} else {
		COULANG_INTERPRETER_ERROR("call frame overflow");
	}
}

CoulangInterpreterCallFrame
pop_call_frame__CoulangInterpreter()
{
	if (interpreter_stack.call_frames_len > 0) {
		return interpreter_stack.call_frames[--interpreter_stack.call_frames_len];
	}

	COULANG_UNREACHABLE("unable to get the top call frame");
}

void
add_current_scope__CoulangInterpreter()
{
	CoulangScope **current_scope_ref = CURRENT_SCOPE_REF();
	CoulangScope *new_scope = init__CoulangScope();

	new_scope->parent = *current_scope_ref;
	*current_scope_ref = new_scope;
}

void
remove_current_scope__CoulangInterpreter()
{
	CoulangScope **current_scope_ref = CURRENT_SCOPE_REF();
	CoulangScope *old_current_scope = *current_scope_ref;

	*current_scope_ref = old_current_scope->parent;

	deinit__CoulangScope(old_current_scope);
}

void
compute_body__CoulangInterpreter(const CoulangDeclFunctionBody *body)
{
	CoulangInterpreterCallFrame *current_frame = CURRENT_FRAME();

  for (size_t i = 0; i < body->len && !is_initialized__CoulangValue(&current_frame->return_value); ++i) {
    const CoulangDeclFunctionBodyItem *item = &body->items[i];

	switch (item->kind) {
		case COULANG_DECL_FUNCTION_BODY_ITEM_KIND_DECL:
			if (item->decl->kind == COULANG_DECL_KIND_VARIABLE) {
				handle_variable_decl__CoulangInterpreter(item->decl);
			} else {
				COULANG_UNREACHABLE("this declaration is not expected");
			}

			break;
		case COULANG_DECL_FUNCTION_BODY_ITEM_KIND_STMT:
			coulang_stmt__CoulangInterpreter(&item->stmt);

			break;
		case COULANG_DECL_FUNCTION_BODY_ITEM_KIND_EXPR: {
			CoulangValue value = compute_value_from_expr__CoulangInterpreter(item->expr);

			deinit__CoulangValue(&value);

			break;
		}
		default:
			COULANG_UNREACHABLE("unknown item kind");
	}
  }
}

void
compute_child_body__CoulangInterpreter(const CoulangDeclFunctionBody *body)
{
  add_current_scope__CoulangInterpreter();
  compute_body__CoulangInterpreter(body);
  remove_current_scope__CoulangInterpreter();
}

void
compute_function_body__CoulangInterpreter(CoulangExpr *called_params, CoulangDeclFunctionParam *decl_params, const CoulangDeclFunctionBody *body)
{
  push_call_frame__CoulangInterpreter(called_params, decl_params);
  compute_body__CoulangInterpreter(body);
}

void run__CoulangInterpreter(const CoulangDeclFunctionBody *body) {
  interpreter_stack = init__CoulangInterpreterStack();

  handle_decls__CoulangInterpreter(body);

  String main_fn_name = init_from_raw__String("main");
  CoulangFunction *main_function = NULL;

  if ((main_function = get_function__CoulangScope(interpreter_stack.global_scope, &main_fn_name))) {
	  compute_function_body__CoulangInterpreter(NULL, NULL, &main_function->decl->body);

	  CoulangInterpreterCallFrame main_frame = pop_call_frame__CoulangInterpreter();

	  deinit__CoulangInterpreterCallFrame(&main_frame);
  } else {
	  COULANG_INTERPRETER_ERROR("expected to have a main function");
  }

  deinit__String(&main_fn_name);
  deinit__CoulangInterpreterStack(&interpreter_stack);
}
