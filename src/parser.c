#include "coulang/ast.h"
#include "coulang/macros.h"
#include <coulang/parser.h>
#include <coulang/token.h>
#include <stdio.h>

static enum CoulangExprBinaryKind tokens_to_binary_kind[COULANG_TOKEN_KIND_MAX] = {
      [COULANG_TOKEN_KIND_STAR] = COULANG_EXPR_BINARY_KIND_MUL,
      [COULANG_TOKEN_KIND_SLASH] = COULANG_EXPR_BINARY_KIND_DIV,
      [COULANG_TOKEN_KIND_MODULO] = COULANG_EXPR_BINARY_KIND_MOD,
      [COULANG_TOKEN_KIND_PLUS] = COULANG_EXPR_BINARY_KIND_ADD,
      [COULANG_TOKEN_KIND_MINUS] = COULANG_EXPR_BINARY_KIND_SUB,
      [COULANG_TOKEN_KIND_LSHIFT] = COULANG_EXPR_BINARY_KIND_LESS,
      [COULANG_TOKEN_KIND_LSHIFT_EQ] = COULANG_EXPR_BINARY_KIND_LESS_EQ,
      [COULANG_TOKEN_KIND_RSHIFT] = COULANG_EXPR_BINARY_KIND_GREATER,
      [COULANG_TOKEN_KIND_RSHIFT_EQ] = COULANG_EXPR_BINARY_KIND_GREATER_EQ,
      [COULANG_TOKEN_KIND_EQ_EQ] = COULANG_EXPR_BINARY_KIND_EQ,
      [COULANG_TOKEN_KIND_BANG_EQ] = COULANG_EXPR_BINARY_KIND_NOT_EQ};

static bool is_done__CoulangToken(TokensIterator *self);

static CoulangToken *consume_token__TokensIterator(TokensIterator *self);

static CoulangToken *get_current_token__TokensIterator(TokensIterator *self);

static CoulangToken *expect_token(enum CoulangTokenKind expected_token,
                                  TokensIterator *ite);

static enum CoulangDataType parse_data_type__Parser(TokensIterator *ite);

static CoulangExpr *parse_list(TokensIterator *ite);

static CoulangExpr *parse_primary_expr(TokensIterator *ite);

static CoulangExpr *parse_unary_expr(TokensIterator *ite);

static CoulangExpr *parse_equality_expr(TokensIterator *ite, CoulangExpr *left);

static CoulangExpr *parse_comparison_expr(TokensIterator *ite, CoulangExpr *left);

static CoulangExpr *parse_add_and_sub_expr(TokensIterator *ite, CoulangExpr *left);

static CoulangExpr *parse_term_expr(TokensIterator *ite, CoulangExpr *left);

static CoulangExpr *parse_binary_expr(TokensIterator *ite, CoulangExpr *left);

static CoulangExpr *parse_expr(TokensIterator *ite);

static CoulangStmt parse_stmt_while(TokensIterator *ite);

static CoulangStmt parse_stmt_if(TokensIterator *ite);

static CoulangStmt parse_stmt_return(TokensIterator *ite);

static CoulangDeclFunctionBody parse_function_body__Parser(TokensIterator *ite);

static CoulangDeclFunctionParam *
parse_function_params_declaration__Parser(TokensIterator *ite);

static CoulangDecl *parse_function_declaration__Parser(TokensIterator *ite);

static CoulangDecl *parse_load_declaration__Parser(TokensIterator *ite);

static CoulangDecl *parse_variable_declaration__Parser(TokensIterator *ite);

static CoulangDecl *parse_declaration__Parser(TokensIterator *ite);

bool is_done__CoulangToken(TokensIterator *self) {
  if (self->content_pos < self->tokens->len - 1) {
    return false;
  }
  return true;
}

// consume the token or return the last one if there all consume
CoulangToken *consume_token__TokensIterator(TokensIterator *self) {
  if (!is_done__CoulangToken(self)) {
    return &self->tokens->buffer[self->content_pos++];
  }
  return &self->tokens->buffer[self->tokens->len - 1];
}

CoulangToken *get_current_token__TokensIterator(TokensIterator *self) {
  return &self->tokens->buffer[self->content_pos];
}

CoulangToken *expect_token(enum CoulangTokenKind expected_token,
                           TokensIterator *ite) {
  CoulangToken *current_token = consume_token__TokensIterator(ite);
  if (current_token->kind != expected_token) {
    COULANG_ERROR("UNEXPECTED TOKEN YOU SHUT");
  }
  return current_token;
}

enum CoulangDataType parse_data_type__Parser(TokensIterator *ite) {
  CoulangToken *token = consume_token__TokensIterator(ite);

  switch (token->kind) {
  case COULANG_TOKEN_KIND_KEYWORD_INT:
    return COULANG_DATA_TYPE_INT;
  case COULANG_TOKEN_KIND_KEYWORD_FLOAT:
    return COULANG_DATA_TYPE_FLOAT;
  case COULANG_TOKEN_KIND_KEYWORD_STR:
    return COULANG_DATA_TYPE_LIST;
  case COULANG_TOKEN_KIND_KEYWORD_LIST:
    return COULANG_DATA_TYPE_STR;
  case COULANG_TOKEN_KIND_KEYWORD_PTR:
    return COULANG_DATA_TYPE_PTR;
  default:
    COULANG_INTERPRETER_ERROR("unknown data type");
  }
}

CoulangExpr *parse_list(TokensIterator *ite) {
  expect_token(COULANG_TOKEN_KIND_LHOOK, ite);

  CoulangExpr *head = NULL;
  CoulangExpr *tail = NULL;
  CoulangToken *current = get_current_token__TokensIterator(ite);

  while (current->kind != COULANG_TOKEN_KIND_RHOOK) {
	CoulangExpr *expr = parse_expr(ite);

	add__CoulangExpr(expr, &head, &tail);

	current = get_current_token__TokensIterator(ite);

    if (current->kind == COULANG_TOKEN_KIND_COMMA) {
      consume_token__TokensIterator(ite);
	  current = get_current_token__TokensIterator(ite);
	} else {
		break;
	}
  }

  expect_token(COULANG_TOKEN_KIND_RHOOK, ite);

  return init_list__CoulangExpr(init__CoulangExprList(head));
}

CoulangExpr *parse_primary_expr(TokensIterator *ite) {
  CoulangToken *current_token = get_current_token__TokensIterator(ite);
  CoulangExpr *expr = NULL;

  switch (current_token->kind) {
  case COULANG_TOKEN_KIND_INTEGER:
	// FIXME: Use another thing than atoi, should return uint64_t
	expr = init_integer__CoulangExpr(atoi(current_token->integer.buffer));

    break;
  case COULANG_TOKEN_KIND_FLOAT:
    expr = init_float__CoulangExpr(atof(current_token->float_.buffer));

    break;
  case COULANG_TOKEN_KIND_STRING:
    expr = init_string__CoulangExpr(&current_token->string);

    break;
  case COULANG_TOKEN_KIND_KEYWORD_LIST:
    return parse_list(ite);
  case COULANG_TOKEN_KIND_BANG:
  case COULANG_TOKEN_KIND_MINUS:
    return parse_unary_expr(ite);
  case COULANG_TOKEN_KIND_IDENTIFIER:
    expr = init_identifier__CoulangExpr(&current_token->identifier);

    break;
  case COULANG_TOKEN_KIND_LHOOK:
	return parse_list(ite);
  case COULANG_TOKEN_KIND_LPAREN: {
	consume_token__TokensIterator(ite);

	expr = parse_expr(ite);

	expect_token(COULANG_TOKEN_KIND_RPAREN, ite);

	return expr;
  }
  default:
    COULANG_INTERPRETER_ERROR("unknown expression");
  }

  consume_token__TokensIterator(ite);

  return expr;
}

CoulangExpr *parse_unary_expr(TokensIterator *ite) {
	static enum CoulangExprUnaryKind tokens_to_unary_kind[] = {
		[COULANG_TOKEN_KIND_BANG] = COULANG_EXPR_UNARY_KIND_NOT,
		[COULANG_TOKEN_KIND_MINUS] = COULANG_EXPR_UNARY_KIND_NEG
	};

    CoulangToken *op_token = consume_token__TokensIterator(ite);
	enum CoulangExprUnaryKind unary_kind = tokens_to_unary_kind[op_token->kind];
	CoulangExpr *right = parse_primary_expr(ite);

	return init_unary__CoulangExpr(init__CoulangExprUnary(unary_kind, right));
}

#define PARSE_RIGHT() \
	  enum CoulangExprBinaryKind binary_kind = tokens_to_binary_kind[current_token->kind]; \
\
	  consume_token__TokensIterator(ite); \
\
	  CoulangExpr *right = parse_primary_expr(ite); \
\
	  left = init_binary__CoulangExpr(init__CoulangExprBinary(binary_kind, left, right)); \
\
     current_token = get_current_token__TokensIterator(ite);


CoulangExpr *parse_equality_expr(TokensIterator *ite, CoulangExpr *left)
{
  CoulangToken *current_token = get_current_token__TokensIterator(ite);

  while (current_token->kind == COULANG_TOKEN_KIND_EQ_EQ ||
		 current_token->kind == COULANG_TOKEN_KIND_BANG_EQ) {
	  PARSE_RIGHT();
  }

  return parse_comparison_expr(ite, left);
}

CoulangExpr *parse_comparison_expr(TokensIterator *ite, CoulangExpr *left)
{
  CoulangToken *current_token = get_current_token__TokensIterator(ite);

  while (current_token->kind == COULANG_TOKEN_KIND_LSHIFT ||
		 current_token->kind == COULANG_TOKEN_KIND_RSHIFT ||
		 current_token->kind == COULANG_TOKEN_KIND_LSHIFT_EQ ||
		 current_token->kind == COULANG_TOKEN_KIND_RSHIFT_EQ) {
	  PARSE_RIGHT();
  }

  return parse_add_and_sub_expr(ite, left);
}

CoulangExpr *parse_add_and_sub_expr(TokensIterator *ite, CoulangExpr *left)
{
  CoulangToken *current_token = get_current_token__TokensIterator(ite);

  while (current_token->kind == COULANG_TOKEN_KIND_PLUS ||
		 current_token->kind == COULANG_TOKEN_KIND_MINUS) {
	  PARSE_RIGHT();
  }

  return parse_term_expr(ite, left);
}

CoulangExpr *parse_term_expr(TokensIterator *ite, CoulangExpr *left)
{
	CoulangToken *current_token = get_current_token__TokensIterator(ite);

	while (current_token->kind == COULANG_TOKEN_KIND_STAR ||
		 current_token->kind == COULANG_TOKEN_KIND_SLASH ||
		 current_token->kind == COULANG_TOKEN_KIND_MODULO) {
		PARSE_RIGHT();
	}

	return left;
}

#undef PARSE_RIGHT

CoulangExpr *parse_binary_expr(TokensIterator *ite, CoulangExpr *left) {
	CoulangExpr *binary_expr = parse_equality_expr(ite, left);

	return tokens_to_binary_kind[get_current_token__TokensIterator(ite)->kind] ? parse_binary_expr(ite, binary_expr) : binary_expr;
}

CoulangExpr *parse_expr(TokensIterator *ite) {
  CoulangExpr *expr = parse_primary_expr(ite);
  CoulangToken *current = get_current_token__TokensIterator(ite);

  switch (current->kind) {
  case COULANG_TOKEN_KIND_PLUS:
  case COULANG_TOKEN_KIND_MINUS:
  case COULANG_TOKEN_KIND_STAR:
  case COULANG_TOKEN_KIND_SLASH:
  case COULANG_TOKEN_KIND_MODULO:
  case COULANG_TOKEN_KIND_EQ:
  case COULANG_TOKEN_KIND_EQ_EQ:
  case COULANG_TOKEN_KIND_BANG_EQ:
  case COULANG_TOKEN_KIND_LSHIFT:
  case COULANG_TOKEN_KIND_RSHIFT:
  case COULANG_TOKEN_KIND_LSHIFT_EQ:
  case COULANG_TOKEN_KIND_RSHIFT_EQ:
    expr = parse_binary_expr(ite, expr);

    break;
  default:
	break;
  }

  return expr;
}

CoulangStmt parse_stmt_while(TokensIterator *ite) {
  consume_token__TokensIterator(ite);

  CoulangExpr *cond = parse_expr(ite);
  CoulangDeclFunctionBody body = parse_function_body__Parser(ite);
  CoulangDeclFunctionBody *body_p =
      COULANG_ALLOC(sizeof(CoulangDeclFunctionBody));

  *body_p = body;

  return init_while__CoulangStmt(init__CoulangStmtWhile(cond, body_p));
}

CoulangStmtIfBranch *parse_stmt_if_branch(TokensIterator *ite) {
  CoulangStmtIfBranch *head = NULL;
  CoulangStmtIfBranch *tail = NULL;
  CoulangToken *current = get_current_token__TokensIterator(ite);

  while (current->kind == COULANG_TOKEN_KIND_KEYWORD_IF ||
         current->kind == COULANG_TOKEN_KIND_KEYWORD_ELIF) {
	consume_token__TokensIterator(ite);

    CoulangExpr *cond = parse_expr(ite);
	CoulangDeclFunctionBody *body_p = COULANG_ALLOC(sizeof(CoulangDeclFunctionBody));
	*body_p = parse_function_body__Parser(ite);
    CoulangStmtIfBranch *branch_p = COULANG_ALLOC(sizeof(CoulangStmtIfBranch));
	*branch_p = init__CoulangStmtIfBranch(cond, body_p);

    add__CoulangStmtIfBranch(branch_p, &head, &tail);

	current = get_current_token__TokensIterator(ite);
  }

  return head;
}

CoulangStmt parse_stmt_if(TokensIterator *ite) {
  CoulangStmtIfBranch *ifs = parse_stmt_if_branch(ite);
  CoulangDeclFunctionBody *else_p = NULL;

  if (get_current_token__TokensIterator(ite)->kind ==
      COULANG_TOKEN_KIND_KEYWORD_ELSE) {
    consume_token__TokensIterator(ite);
	else_p = COULANG_ALLOC(sizeof(CoulangDeclFunctionBody));
    *else_p = parse_function_body__Parser(ite);
  }

  return init_if__CoulangStmt(init__CoulangStmtIf(ifs, else_p));
}

CoulangStmt parse_stmt_return(TokensIterator *ite) {
  consume_token__TokensIterator(ite);

  CoulangExpr *expr = parse_expr(ite);

  return init_return__CoulangStmt(expr);
}

CoulangDeclFunctionBody parse_function_body__Parser(TokensIterator *ite) {
  expect_token(COULANG_TOKEN_KIND_LBRACE, ite);

  CoulangDeclFunctionBody body = init__CoulangDeclFunctionBody();
  CoulangToken *current = get_current_token__TokensIterator(ite);

  while (current->kind != COULANG_TOKEN_KIND_RBRACE) {
    CoulangDeclFunctionBodyItem item = {0};

    switch (current->kind) {
    case COULANG_TOKEN_KIND_KEYWORD_WHILE:
      item = init_stmt__CoulangDeclFunctionBodyItem(parse_stmt_while(ite));
      break;
	case COULANG_TOKEN_KIND_KEYWORD_IF:
      item = init_stmt__CoulangDeclFunctionBodyItem(parse_stmt_if(ite));
      break;
	case COULANG_TOKEN_KIND_KEYWORD_RETURN:
      item = init_stmt__CoulangDeclFunctionBodyItem(parse_stmt_return(ite));
      break;
	case COULANG_TOKEN_KIND_KEYWORD_VAL:
      item = init_decl__CoulangDeclFunctionBodyItem(parse_variable_declaration__Parser(ite));
	  break;
    default:
      item = init_expr__CoulangDeclFunctionBodyItem(parse_expr(ite));
    }

    add__CoulangDeclFunctionBody(&body, item);

	current = get_current_token__TokensIterator(ite);
  }

  expect_token(COULANG_TOKEN_KIND_RBRACE, ite);

  return body;
}

CoulangDeclFunctionParam *
parse_function_params_declaration__Parser(TokensIterator *ite) {
  expect_token(COULANG_TOKEN_KIND_LPAREN, ite);

  CoulangDeclFunctionParam *head = NULL;
  CoulangDeclFunctionParam *tail = NULL;
  CoulangToken *current = get_current_token__TokensIterator(ite);

  while (current->kind != COULANG_TOKEN_KIND_RPAREN) {
    CoulangToken *token_identifier =
        expect_token(COULANG_TOKEN_KIND_IDENTIFIER, ite);
    const String *const name = &token_identifier->identifier;
    enum CoulangDataType data_type = parse_data_type__Parser(ite);

    CoulangToken *current = get_current_token__TokensIterator(ite);

    CoulangDeclFunctionParam *param =
        init__CoulangDeclFunctionParam(name, data_type);

    add__CoulangDeclFunctionParam(param, &head, &tail);

    if (current->kind == COULANG_TOKEN_KIND_COMMA) {
      consume_token__TokensIterator(ite);
    } else {
      break;
    }
  }

  expect_token(COULANG_TOKEN_KIND_RPAREN, ite);

  return head;
}

CoulangDecl *parse_function_declaration__Parser(TokensIterator *ite) {
  consume_token__TokensIterator(ite);
  CoulangToken *token = expect_token(COULANG_TOKEN_KIND_IDENTIFIER, ite);
  const String *name = &token->identifier;
  CoulangDeclFunctionParam *params =
      parse_function_params_declaration__Parser(ite);
  enum CoulangDataType return_data_type = parse_data_type__Parser(ite);
  CoulangDeclFunctionBody body = parse_function_body__Parser(ite);
  CoulangDecl decl = init_function__CoulangDecl(init__CoulangDeclFunction(name, params, return_data_type, body));
  CoulangDecl *decl_p = COULANG_ALLOC(sizeof(CoulangDecl));

  *decl_p = decl;

  return decl_p;
}

CoulangDecl *parse_load_declaration__Parser(TokensIterator *ite) {
  consume_token__TokensIterator(ite);

  CoulangToken *library = expect_token(COULANG_TOKEN_KIND_STRING, ite);
  const String *library_name = &library->string;
  CoulangToken *current = get_current_token__TokensIterator(ite);
  Strings symbols = init__Strings();

  while (current->kind == COULANG_TOKEN_KIND_IDENTIFIER) {
	  add__Strings(&symbols, init_copy__String(&current->identifier));
	  consume_token__TokensIterator(ite);

	  current = get_current_token__TokensIterator(ite);

	  if (current->kind == COULANG_TOKEN_KIND_COMMA) {
		  consume_token__TokensIterator(ite);
		  current = get_current_token__TokensIterator(ite);
	  } else {
		  break;
	  }
  }

  CoulangDecl decl = init_load__CoulangDecl(init__CoulangDeclLoad(library_name, symbols));
  CoulangDecl *decl_p = COULANG_ALLOC(sizeof(CoulangDecl));

  *decl_p = decl;

  return decl_p;
}

CoulangDecl *parse_variable_declaration__Parser(TokensIterator *ite) {
  consume_token__TokensIterator(ite);

  CoulangToken *token = expect_token(COULANG_TOKEN_KIND_IDENTIFIER, ite);
  const String *name = &token->identifier;
  enum CoulangDataType data_type = parse_data_type__Parser(ite);

  expect_token(COULANG_TOKEN_KIND_EQ, ite);

  CoulangExpr *expr = parse_expr(ite);
  CoulangDecl decl = init_variable__CoulangDecl(init__CoulangDeclVariable(name, data_type, expr));
  CoulangDecl *decl_p = COULANG_ALLOC(sizeof(CoulangDecl));

  *decl_p = decl;

  return decl_p;
}

CoulangDecl *parse_declaration__Parser(TokensIterator *ite) {
  CoulangToken *front_token = get_current_token__TokensIterator(ite);

  switch (front_token->kind) {
  case COULANG_TOKEN_KIND_KEYWORD_FN:
    return parse_function_declaration__Parser(ite);
  case COULANG_TOKEN_KIND_KEYWORD_LOAD:
    return parse_load_declaration__Parser(ite);
  case COULANG_TOKEN_KIND_KEYWORD_VAL:
    return parse_variable_declaration__Parser(ite);
  default:
    COULANG_INTERPRETER_ERROR("unexpected token");
  }
}

void parse_declarations__Parser(TokensIterator *ite,
                                CoulangDeclFunctionBody *body) {
  while (!is_done__CoulangToken(ite)) {
	CoulangDeclFunctionBodyItem item = init_decl__CoulangDeclFunctionBodyItem(parse_declaration__Parser(ite));

    add__CoulangDeclFunctionBody(body, item);
  }
}
