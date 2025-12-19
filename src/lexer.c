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

#include <coulang/lexer.h>
#include <coulang/macros.h>
#include <coulang/string.h>
#include <coulang/token.h>
#include <ctype.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void err(char *error) {
  printf("%s\n", error);
  exit(-1);
}

typedef struct {
  char *content;
  size_t content_pos;
  size_t size;
  Position code_pos;
} FileIterator;

FileIterator read_file__FileIterator(const char *path) {
  FILE *f = fopen(path, "r");
  if (f == NULL) {
    COULANG_ERROR("error while opening the file");
  }

  struct stat s;
  if (stat(path, &s) == -1) {
    COULANG_ERROR("error while reading the size of the file");
  }

  char *buffer = COULANG_ALLOC(s.st_size + 1);

  if (fread(buffer, 1, s.st_size, f) != s.st_size) {
    COULANG_ERROR("error while reading the file");
  }
  buffer[s.st_size] = '\0';

  fclose(f);

  return (FileIterator){.content = buffer,
                        .content_pos = 0,
                        .size = s.st_size,
                        .code_pos = (Position){
                            .colomne = 1,
                            .line = 1,
                        }};
}

int is_done__FileIterator(FileIterator *file) {
  if (file->content_pos < file->size) {
    return 0;
  }

  return 1;
}

/*
 * return -1 if the file is done
 */
char consume_char__FileIterator(FileIterator *self) {
  if (!is_done__FileIterator(self)) {
    char c = self->content[self->content_pos++];
    self->code_pos.colomne++;
    if (c == '\n') {
      self->code_pos.line += 1;
      self->code_pos.colomne = 1;
    }
    return c;
  }
  return -1;
}

void restore_char__FileIteratro(FileIterator *self) {
  if (self->content_pos > 0 && self->content[self->content_pos - 1] != '\n') {
    self->content_pos--;
    self->code_pos.colomne--;
  }
}

// return -1 if there is no next char
char get_next_char__FileIterator(FileIterator *self) {
  if (self->content_pos < self->size - 1) {
    return self->content[self->content_pos + 1];
  }
  return -1;
}

char get_current_char__FileIterator(FileIterator *self) {
  return self->content[self->content_pos];
}

// return -1 if there is not privious char
char get_privious_char__FileIterator(FileIterator *self) {
  if (self->content_pos > 0) {
    return self->content[self->content_pos - 1];
  }
  return -1;
}

String get_next_identifier__FileIterator(FileIterator *self) {
  char current_char = get_privious_char__FileIterator(self);
  String s = init__String();
  while (isalnum(current_char) || current_char == '_') {
    add__String(&s, current_char);
    current_char = consume_char__FileIterator(self);
  }
  restore_char__FileIteratro(self);
  return s;
}

void skip_space__FileIterator(FileIterator *self) {
  char current_char = consume_char__FileIterator(self);
  while (isspace(current_char)) {
    current_char = consume_char__FileIterator(self);
  }
  restore_char__FileIteratro(self);
}

void deinit__FileIterator(FileIterator *file) { free(file->content); }

// get the keyword associated with the string and return a id if it's not an
// id
enum CoulangTokenKind get_keyword(String s) {
  if (strcmp(s.buffer, "if") == 0) {
    return COULANG_TOKEN_KIND_KEYWORD_IF;
  }
  if (strcmp(s.buffer, "else") == 0) {
    return COULANG_TOKEN_KIND_KEYWORD_ELSE;
  }
  if (strcmp(s.buffer, "elif") == 0) {
    return COULANG_TOKEN_KIND_KEYWORD_ELIF;
  }
  if (strcmp(s.buffer, "int") == 0) {
    return COULANG_TOKEN_KIND_KEYWORD_INT;
  }
  if (strcmp(s.buffer, "float") == 0) {
    return COULANG_TOKEN_KIND_KEYWORD_FLOAT;
  }
  if (strcmp(s.buffer, "load") == 0) {
    return COULANG_TOKEN_KIND_KEYWORD_LOAD;
  }
  if (strcmp(s.buffer, "str") == 0) {
    return COULANG_TOKEN_KIND_KEYWORD_STR;
  }
  if (strcmp(s.buffer, "list") == 0) {
    return COULANG_TOKEN_KIND_KEYWORD_LIST;
  }
  if (strcmp(s.buffer, "fn") == 0) {
    return COULANG_TOKEN_KIND_KEYWORD_FN;
  }
  if (strcmp(s.buffer, "while") == 0) {
    return COULANG_TOKEN_KIND_KEYWORD_WHILE;
  }
  if (strcmp(s.buffer, "ptr") == 0) {
    return COULANG_TOKEN_KIND_KEYWORD_PTR;
  }
  if (strcmp(s.buffer, "return") == 0) {
    return COULANG_TOKEN_KIND_KEYWORD_RETURN;
  }
  if (strcmp(s.buffer, "val") == 0) {
    return COULANG_TOKEN_KIND_KEYWORD_VAL;
  }
  return COULANG_TOKEN_KIND_IDENTIFIER;
}

CoulangTokens lex(const char *path) {
  FileIterator file = read_file__FileIterator(path);
  CoulangToken token = {0};
  CoulangTokens tokens = init__CoulangTokens();

  while (!is_done__FileIterator(&file)) {
    skip_space__FileIterator(&file);
    char current_char = consume_char__FileIterator(&file);
    if (is_done__FileIterator(&file)) {
      break;
    }
    switch (current_char) {
    case '+':
      token = init__CoulangToken(COULANG_TOKEN_KIND_PLUS, file.code_pos);
      break;
    case '-':
      token = init__CoulangToken(COULANG_TOKEN_KIND_MINUS, file.code_pos);
      break;
    case '/':
      token = init__CoulangToken(COULANG_TOKEN_KIND_SLASH, file.code_pos);
      break;
    case '*':
      token = init__CoulangToken(COULANG_TOKEN_KIND_STAR, file.code_pos);
      break;
    case '%':
      token = init__CoulangToken(COULANG_TOKEN_KIND_MODULO, file.code_pos);
      break;
    case '{':
      token = init__CoulangToken(COULANG_TOKEN_KIND_LBRACE, file.code_pos);
      break;
    case '}':
      token = init__CoulangToken(COULANG_TOKEN_KIND_RBRACE, file.code_pos);
      break;
    case '(':
      token = init__CoulangToken(COULANG_TOKEN_KIND_LPAREN, file.code_pos);
      break;
    case ')':
      token = init__CoulangToken(COULANG_TOKEN_KIND_RPAREN, file.code_pos);
      break;
    case '[':
      token = init__CoulangToken(COULANG_TOKEN_KIND_LHOOK, file.code_pos);
      break;
    case ']':
      token = init__CoulangToken(COULANG_TOKEN_KIND_RHOOK, file.code_pos);
      break;
    case '=':
      if (get_current_char__FileIterator(&file) == '=') {
        Position temp = file.code_pos;
        consume_char__FileIterator(&file);
        token = init__CoulangToken(COULANG_TOKEN_KIND_EQ_EQ, temp);
        break;
      }
      token = init__CoulangToken(COULANG_TOKEN_KIND_EQ, file.code_pos);
      break;
    case '!':
      if (get_current_char__FileIterator(&file) == '=') {
        Position temp = file.code_pos;
        consume_char__FileIterator(&file);
        token = init__CoulangToken(COULANG_TOKEN_KIND_BANG_EQ, temp);
        break;
      }
      token = init__CoulangToken(COULANG_TOKEN_KIND_BANG, file.code_pos);
      break;
    case '<':
      if (get_current_char__FileIterator(&file) == '=') {
        Position temp = file.code_pos;
        consume_char__FileIterator(&file);
        token = init__CoulangToken(COULANG_TOKEN_KIND_LSHIFT_EQ, temp);
        break;
      }
      token = init__CoulangToken(COULANG_TOKEN_KIND_LSHIFT, file.code_pos);
      break;
    case '>':
      if (get_current_char__FileIterator(&file) == '=') {
        Position temp = file.code_pos;
        consume_char__FileIterator(&file);
        token = init__CoulangToken(COULANG_TOKEN_KIND_RSHIFT_EQ, temp);
        break;
      }
      token = init__CoulangToken(COULANG_TOKEN_KIND_RSHIFT, file.code_pos);
      break;
    case ',':
      token = init__CoulangToken(COULANG_TOKEN_KIND_COMMA, file.code_pos);
      break;
    case '\n':
      continue;
    case '"':
      String s = init__String();
      Position temp = file.code_pos;
      current_char = consume_char__FileIterator(&file);
      while (current_char != '"') {
        add__String(&s, current_char);
        current_char = consume_char__FileIterator(&file);
        if (is_done__FileIterator(&file)) {
          COULANG_INTERPRETER_ERROR("MISSING THE ENDING \" SON OF A BEACH");
        }
      }
      token = init_string__CoulangToken(s, temp);
      break;
    case '0' ... '9': {
      String s = init__String();
      bool is_float = false;
      Position temp = file.code_pos;
      while (isdigit(current_char)) {
        if (get_current_char__FileIterator(&file) == '.') {
          add__String(&s, current_char);
          current_char = consume_char__FileIterator(&file);
          if (!is_float) {
            is_float = true;
          } else {
            COULANG_INTERPRETER_ERROR(
                "the digit has to much point mother fucker at: %zu, %zu",
                file.code_pos.line, file.code_pos.colomne);
          }
        }
        add__String(&s, current_char);
        current_char = consume_char__FileIterator(&file);
      }
      restore_char__FileIteratro(&file);
      if (is_float) {
        token = init_float__CoulangToken(s, temp);
      } else {
        token = init_integer__CoulangToken(s, temp);
      }
      break;
    }
    case 'a' ... 'z': {
      String s = get_next_identifier__FileIterator(&file);
      enum CoulangTokenKind token_kind = get_keyword(s);
      Position temp = file.code_pos;
      if (token_kind == COULANG_TOKEN_KIND_IDENTIFIER) {
        token = init_identifier__CoulangToken(s, temp);
      } else {
        deinit__String(&s);
        token = init__CoulangToken(token_kind, temp);
      }
      break;
    }
    case 'A' ... 'Z': {
      Position temp = file.code_pos;
      String s = get_next_identifier__FileIterator(&file);
      token = init_identifier__CoulangToken(s, temp);
      break;
    }
    case '_': {
      Position temp = file.code_pos;
      String s = get_next_identifier__FileIterator(&file);
      token = init_identifier__CoulangToken(s, temp);
      break;
    }
    case '#':
      // read til the end of the line to ignore the comments
      while (current_char != '\n') {
        current_char = consume_char__FileIterator(&file);
      }
      continue;
    case '\0':
      break;
    default:
      COULANG_INTERPRETER_ERROR("WTF HAVE YOU DONE HERE: %zu, %zu",
                                file.code_pos.line, file.code_pos.colomne);
    }

    add__CoulangTokens(&tokens, token);
  }

  add__CoulangTokens(&tokens,
                     init__CoulangToken(COULANG_TOKEN_KIND_EOF, file.code_pos));

  deinit__FileIterator(&file);
  return tokens;
}
