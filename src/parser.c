#include "coulang/ast.h"
#include "coulang/macros.h"
#include <coulang/parser.h>
#include <coulang/token.h>
#include <stdio.h>

CoulangExpr parse_expr(TokensIterator *ite);

bool is_done__CoulangToken(TokensIterator *self) {
  if (self->content_pos < self->tokens->len) {
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

void restor_token__TokensIterator(TokensIterator *self) {
  if (self->content_pos > 0) {
    self->content_pos--;
  }
}

CoulangToken *get_privious_token__TokensIterator(TokensIterator *self) {
  return &self->tokens->buffer[self->content_pos - 1];
}

CoulangToken *get_current_token__TokensIterator(TokensIterator *self) {
  return &self->tokens->buffer[self->content_pos];
}

CoulangToken *get_next_token__TokensIterator(TokensIterator *self) {
  return &self->tokens->buffer[self->content_pos + 1];
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

CoulangExpr parse_list(TokensIterator *ite) {
  expect_token(COULANG_TOKEN_KIND_LHOOK, ite);
  CoulangExprList *head = NULL;
  CoulangExprList *tail = NULL;
  CoulangToken *current = get_current_token__TokensIterator(ite);

  while (current->kind != COULANG_TOKEN_KIND_RHOOK) {
    parse_expr(ite);
  }
}

CoulangExpr parse_primary_expr(TokensIterator *ite) {
  switch (consume_token__TokensIterator(ite)->kind) {
  case COULANG_TOKEN_KIND_INTEGER:

  case COULANG_TOKEN_KIND_FLOAT:

  case COULANG_TOKEN_KIND_STRING:

  case COULANG_TOKEN_KIND_KEYWORD_LIST:
    parse_list(ite);
    break;
  }
}

CoulangExpr parse_expr(TokensIterator *ite) {}

CoulangStmt parse_stmt_while(TokensIterator *ite) {}

CoulangStmt parse_stmt_if(TokensIterator *ite) {}

CoulangStmt parse_stmt_return(TokensIterator *ite) {}

CoulangDeclFunctionBody parse_function_body__Parser(TokensIterator *ite) {
  expect_token(COULANG_TOKEN_KIND_LBRACE, ite);
  CoulangDeclFunctionBody *head = NULL;
  CoulangDeclFunctionBody *tail = NULL;
  CoulangToken *current = get_current_token__TokensIterator(ite);

  while (current->kind != COULANG_TOKEN_KIND_RBRACE) {
    switch (current->kind) {
    case COULANG_STMT_KIND_WHILE:

    case COULANG_STMT_KIND_IF:

    case COULANG_STMT_KIND_RETURN:
    }
  }
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
  CoulangDeclFunctionParam *params =
      parse_function_params_declaration__Parser(ite);
  enum CoulangDataType return_data_type = parse_data_type__Parser(ite);
  CoulangDeclFunctionBody body = parse_function_body__Parser(ite);
}

CoulangDecl *parse_load_declaration__Parser(TokensIterator *ite) {}

CoulangDecl *parse_variable_declaration__Parser(TokensIterator *ite) {}

CoulangDecl *parse_declaration__Parser(TokensIterator *ite) {
  CoulangToken *front_token = get_current_token__TokensIterator(ite);

  switch (front_token->kind) {
  case COULANG_TOKEN_KIND_KEYWORD_FN:
    return parse_function_declaration__Parser(ite);
  case COULANG_TOKEN_KIND_KEYWORD_LOAD:
    return parse_load_declaration__Parser(ite);
  case COULANG_TOKEN_KIND_IDENTIFIER:
    return parse_variable_declaration__Parser(ite);
  default:
    COULANG_INTERPRETER_ERROR("unexpected token");
  }
}
