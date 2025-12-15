#include "coulang/ast.h"
#include "coulang/macros.h"
#include <coulang/parser.h>
#include <coulang/token.h>
#include <stdio.h>

bool is_done__CoulangToken(TokensIterator *self) {
  if (self->content_pos < self->tokens->len) {
    return true;
  }
  return false;
}

// consume the token or return the last one if there all consume
CoulangToken consume_token__TokensIterator(TokensIterator *self) {
  if (!is_done__CoulangToken(self)) {
    return self->tokens->buffer[self->content_pos++];
  }
  return self->tokens->buffer[self->tokens->len - 1];
}

void restor_token__TokensIterator(TokensIterator *self) {
  if (self->content_pos > 0) {
    self->content_pos--;
  }
}

CoulangToken get_privious_token__TokensIterator(TokensIterator *self) {
  return self->tokens->buffer[self->content_pos - 1];
}

CoulangToken get_current_token__TokensIterator(TokensIterator *self) {
  return self->tokens->buffer[self->content_pos];
}

CoulangToken get_next_token__TokensIterator(TokensIterator *self) {
  return self->tokens->buffer[self->content_pos + 1];
}

CoulangToken expect_token(enum CoulangTokenKind expected_token,
                          TokensIterator *ite) {
  CoulangToken current_token = consume_token__TokensIterator(ite);
  if (current_token.kind != expected_token) {
    COULANG_ERROR("UNEXPECTED TOKEN YOU SHUT");
  }
  return current_token;
}

CoulangDecl *parse_function_declaration(TokensIterator *ite) {
  consume_token__TokensIterator(ite);
  CoulangToken token = expect_token(COULANG_TOKEN_KIND_IDENTIFIER, ite);
  printf("%s\n", token.identifier.buffer);
}

CoulangDecl *parse_declaration__Parser(TokensIterator *ite) {
  switch (get_current_token__TokensIterator(ite).kind) {
  case COULANG_TOKEN_KIND_KEYWORD_FN:
    return parse_function_declaration(ite);
  case COULANG_TOKEN_KIND_KEYWORD_LOAD:
    break;
  case COULANG_TOKEN_KIND_IDENTIFIER:
    break;
  }
}
