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

static inline CoulangExprBinary
init__CoulangExprBinary(enum CoulangExprBinaryKind kind,
                        struct CoulangExpr *left, struct CoulangExpr *right) {
  return (CoulangExprBinary){.kind = kind, .left = left, .right = right};
}

void deinit__CoulangExprBinary(const CoulangExprBinary *const self);

typedef struct {
  struct CoulangExpr *head;
} CoulangExprList;

static inline CoulangExprList init__CoulangExprList(struct CoulangExpr *head) {
  return (CoulangExprList){.head = head};
}

void deinit__CoulangExprList(const CoulangExprList *const self);

enum CoulangExprKind {
	COULANG_EXPR_KIND_BINARY,
	COULANG_EXPR_KIND_STRING,
	COULANG_EXPR_KIND_INTEGER,
	COULANG_EXPR_KIND_FLOAT,
	COULANG_EXPR_KIND_LIST,
	COULANG_EXPR_KIND_GROUPING,
	COULANG_EXPR_KIND_IDENTIFIER
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
		CoulangExpr *grouping;
		const String *identifier;
	};
} CoulangExpr;

CoulangExpr *init_binary__CoulangExpr(CoulangExprBinary binary);

CoulangExpr *init_string__CoulangExpr(const String *string);

CoulangExpr *init_integer__CoulangExpr(uint64_t integer);

CoulangExpr *init_float__CoulangExpr(double float_);

CoulangExpr *init_list__CoulangExpr(CoulangExprList list);

CoulangExpr *init_grouping__CoulangExpr(CoulangExpr *grouping);

CoulangExpr *
init_identifier__CoulangExpr(const String *identifier);

static inline void
add__CoulangExpr(CoulangExpr *self, CoulangExpr **head, CoulangExpr **tail)
{
	if (!*head) {
		*head = self;
	}

	if (*tail) {
		(*tail)->next = self;
	}

	*tail = self;
}

void deinit__CoulangExpr(CoulangExpr *self);

enum CoulangDataType {
  COULANG_DATA_TYPE_INT,
  COULANG_DATA_TYPE_FLOAT,
  COULANG_DATA_TYPE_LIST,
  COULANG_DATA_TYPE_STR,
  COULANG_DATA_TYPE_PTR,
};

typedef struct CoulangDeclFunctionBody CoulangDeclFunctionBody;

typedef struct CoulangStmtIfBranch {
  CoulangExpr *cond;
  CoulangDeclFunctionBody *body;
  struct CoulangStmtIfBranch *next;
} CoulangStmtIfBranch;

static inline CoulangStmtIfBranch
init__CoulangStmtIfBranch(CoulangExpr *cond, CoulangDeclFunctionBody *body) {
  return (CoulangStmtIfBranch){.cond = cond, .body = body, .next = NULL};
}

static inline void add__CoulangStmtIfBranch(CoulangStmtIfBranch *self,
                                            CoulangStmtIfBranch **tail) {
  if (*tail) {
    (*tail)->next = self;
  }

  *tail = self;
}

void deinit__CoulangStmtIfBranch(CoulangStmtIfBranch *self);

typedef struct {
  CoulangStmtIfBranch *ifs;
  CoulangDeclFunctionBody *else_;
} CoulangStmtIf;

static inline CoulangStmtIf
init__CoulangStmtIf(CoulangStmtIfBranch *ifs, CoulangDeclFunctionBody *else_) {
  return (CoulangStmtIf){.ifs = ifs, .else_ = else_};
}

void deinit__CoulangStmtIf(const CoulangStmtIf *const self);

typedef struct {
  CoulangExpr *cond;
  CoulangDeclFunctionBody *body;
} CoulangStmtWhile;

static inline CoulangStmtWhile
init__CoulangStmtWhile(CoulangExpr *cond, CoulangDeclFunctionBody *body) {
  return (CoulangStmtWhile){.cond = cond, .body = body};
}

void deinit__CoulangStmtWhile(const CoulangStmtWhile *const self);

enum CoulangStmtKind {
  COULANG_STMT_KIND_IF,
  COULANG_STMT_KIND_RETURN,
  COULANG_STMT_KIND_WHILE,
};

typedef struct {
  enum CoulangStmtKind kind;
  union {
    CoulangStmtIf if_;
    CoulangExpr *return_;
    CoulangStmtWhile while_;
  };
} CoulangStmt;

static inline CoulangStmt init_if__CoulangStmt(CoulangStmtIf if_) {
  return (CoulangStmt){.kind = COULANG_STMT_KIND_IF, .if_ = if_};
}

static inline CoulangStmt init_return__CoulangStmt(CoulangExpr *return_) {
  return (CoulangStmt){.kind = COULANG_STMT_KIND_RETURN, .return_ = return_};
}

static inline CoulangStmt init_while__CoulangStmt(CoulangStmtWhile while_) {
  return (CoulangStmt){.kind = COULANG_STMT_KIND_WHILE, .while_ = while_};
}

void deinit__CoulangStmt(const CoulangStmt *const self);

typedef struct CoulangDecl CoulangDecl;

typedef struct {
  const String *library;
  Strings symbols;
} CoulangDeclLoad;

static inline CoulangDeclLoad init__CoulangDeclLoad(const String *library,
                                                    Strings symbols) {
  return (CoulangDeclLoad){.library = library, .symbols = symbols};
}

static inline void deinit__CoulangDeclLoad(const CoulangDeclLoad *const self) {
  deinit__Strings(&self->symbols);
}

typedef struct CoulangDeclFunctionParam {
  const String *name;
  enum CoulangDataType data_type;
  struct CoulangDeclFunctionParam *next;
} CoulangDeclFunctionParam;

CoulangDeclFunctionParam *
init__CoulangDeclFunctionParam(const String *name,
                               enum CoulangDataType data_type);

static inline void
add__CoulangDeclFunctionParam(CoulangDeclFunctionParam *self,
                              CoulangDeclFunctionParam **head,
                              CoulangDeclFunctionParam **tail) {
  if (!*head) {
    *head = self;
  }

  if (*tail) {
    (*tail)->next = self;
  }

  *tail = self;
}

void deinit__CoulangDeclFunctionParam(CoulangDeclFunctionParam *self);

enum CoulangDeclFunctionBodyItemKind {
  COULANG_DECL_FUNCTION_BODY_ITEM_KIND_EXPR,
  COULANG_DECL_FUNCTION_BODY_ITEM_KIND_STMT,
  COULANG_DECL_FUNCTION_BODY_ITEM_KIND_DECL
};

typedef struct {
  enum CoulangDeclFunctionBodyItemKind kind;
  union {
    CoulangExpr *expr;
    CoulangStmt stmt;
    CoulangDecl *decl;
  };
} CoulangDeclFunctionBodyItem;

static inline CoulangDeclFunctionBodyItem
init_expr__CoulangDeclFunctionBodyItem(CoulangExpr *expr) {
  return (CoulangDeclFunctionBodyItem){
      .kind = COULANG_DECL_FUNCTION_BODY_ITEM_KIND_EXPR,
      .expr = expr,
  };
}

static inline CoulangDeclFunctionBodyItem
init_stmt__CoulangDeclFunctionBodyItem(CoulangStmt stmt) {
  return (CoulangDeclFunctionBodyItem){
      .kind = COULANG_DECL_FUNCTION_BODY_ITEM_KIND_STMT, .stmt = stmt};
}

static inline CoulangDeclFunctionBodyItem
init_decl__CoulangDeclFunctionBodyItem(CoulangDecl *decl) {
  return (CoulangDeclFunctionBodyItem){
      .kind = COULANG_DECL_FUNCTION_BODY_ITEM_KIND_DECL, .decl = decl};
}

void deinit__CoulangDeclFunctionBodyItem(CoulangDeclFunctionBodyItem *self);

typedef struct CoulangDeclFunctionBody {
  CoulangDeclFunctionBodyItem *items;
  size_t len;
  size_t capacity;
} CoulangDeclFunctionBody;

#define COULANG_DECL_FUNCTION_BODY_DEFAULT_CAPACITY 8

static inline CoulangDeclFunctionBody init__CoulangDeclFunctionBody() {
  return (CoulangDeclFunctionBody){
      .items = NULL,
      .len = 0,
      .capacity = COULANG_DECL_FUNCTION_BODY_DEFAULT_CAPACITY};
}

void add__CoulangDeclFunctionBody(CoulangDeclFunctionBody *self,
                                  CoulangDeclFunctionBodyItem item);

void deinit__CoulangDeclFunctionBody(const CoulangDeclFunctionBody *const self);

typedef struct {
  const String *name;
  CoulangDeclFunctionParam *params;
  CoulangDeclFunctionBody body;
} CoulangDeclFunction;

static inline CoulangDeclFunction
init__CoulangDeclFunction(const String *name, CoulangDeclFunctionParam *params,
                          CoulangDeclFunctionBody body) {
  return (CoulangDeclFunction){.name = name, .params = params, .body = body};
}

static inline void
deinit__CoulangDeclFunction(const CoulangDeclFunction *const self) {
  deinit__CoulangDeclFunctionParam(self->params);
  deinit__CoulangDeclFunctionBody(&self->body);
}

typedef struct {
  const String *name;
  enum CoulangDataType data_type;
  CoulangExpr *expr;
} CoulangDeclVariable;

static inline CoulangDeclVariable
init__CoulangDeclVariable(const String *name, enum CoulangDataType data_type,
                          CoulangExpr *expr) {
  return (CoulangDeclVariable){
      .name = name, .data_type = data_type, .expr = expr};
}

static inline void
deinit__CoulangDeclVariable(const CoulangDeclVariable *const self) {
  deinit__CoulangExpr(self->expr);
}

enum CoulangDeclKind {
  COULANG_DECL_KIND_LOAD,
  COULANG_DECL_KIND_FUNCTION,
  COULANG_DECL_KIND_VARIABLE
};

typedef struct CoulangDecl {
  enum CoulangDeclKind kind;
  union {
    CoulangDeclLoad load;
    CoulangDeclFunction function;
    CoulangDeclVariable variable;
  };
} CoulangDecl;

static inline CoulangDecl init_load__CoulangDecl(CoulangDeclLoad load) {
  return (CoulangDecl){.kind = COULANG_DECL_KIND_LOAD, .load = load};
}

static inline CoulangDecl
init_function__CoulangDecl(CoulangDeclFunction function) {
  return (CoulangDecl){.kind = COULANG_DECL_KIND_FUNCTION,
                       .function = function};
}

static inline CoulangDecl
init_variable__CoulangDecl(CoulangDeclVariable variable) {
  return (CoulangDecl){.kind = COULANG_DECL_KIND_VARIABLE,
                       .variable = variable};
}

void deinit__CoulangDecl(const CoulangDecl *const self);

#endif // COULANG_AST_H
