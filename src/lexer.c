#include <coulang/lexer.h>
#include <coulang/macros.h>
#include <coulang/string.h>
#include <coulang/token.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void err(char *error) {
  printf("%s\n", error);
  exit(-1);
}

typedef struct FileIterator {
  char *content;
  size_t current_pos;
  size_t size;
} FileIterator;

FileIterator read_file(char *path) {
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

  return (FileIterator){
      .content = buffer,
      .current_pos = 0,
      .size = s.st_size,
  };
}

int is_done(FileIterator *file) {
  if (file->current_pos < file->size) {
    return 0;
  }

  return 1;
}

/*
 * return -1 if the file is done
 */
char consume_char__FileIterator(FileIterator *self) {
  if (!is_done(self)) {
    return self->content[self->current_pos++];
  }
  return -1;
}

// return -1 if there is no next char
char get_next_char__FileIterator(FileIterator *self) {
  if (self->current_pos < self->size - 2) {
    return self->content[self->current_pos + 1];
  }
  return -1;
}

// return -1 if there is not privious char
char get_privious_char__FileIterator(FileIterator *self) {
  if (self->current_pos > 0) {
    return self->content[self->current_pos - 1];
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
  self->current_pos--;
  return s;
}

void deinit__FileIterator(FileIterator *file) { free(file->content); }

// get the keyword associated with the string and return a id if it's not an id
enum CoulangTokenKind get_keyword(String s) {
  if (strcmp(s.buffer, "if") == 0) {
    return COULANG_TOKEN_KIND_KEYWORD_IF;
  }
  if (strcmp(s.buffer, "else") == 0) {
    return COULANG_TOKEN_KIND_KEYWORD_ELSE;
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
  return COULANG_TOKEN_KIND_IDENTIFIER;
}

void lex(char *path) {
  FileIterator file = read_file(path);
  CoulangToken token = {0};
  CoulangTokens tokens = init__CoulangTokens();

  while (!is_done(&file)) {
    char current_char = consume_char__FileIterator(&file);
    switch (current_char) {
    case '+':
      token = init__CoulangToken(COULANG_TOKEN_KIND_PLUS);
      break;
    case '-':
      token = init__CoulangToken(COULANG_TOKEN_KIND_MINUS);
      break;
    case '/':
      token = init__CoulangToken(COULANG_TOKEN_KIND_SLASH);
      break;
    case '*':
      token = init__CoulangToken(COULANG_TOKEN_KIND_STAR);
      break;
    case '%':
      token = init__CoulangToken(COULANG_TOKEN_KIND_MODULO);
      break;
    case '{':
      token = init__CoulangToken(COULANG_TOKEN_KIND_LBRACE);
      break;
    case '}':
      token = init__CoulangToken(COULANG_TOKEN_KIND_RBRACE);
      break;
    case '(':
      token = init__CoulangToken(COULANG_TOKEN_KIND_LPAREN);
      break;
    case ')':
      token = init__CoulangToken(COULANG_TOKEN_KIND_RPAREN);
      break;
    case '"':
      String s = init__String();
      current_char = consume_char__FileIterator(&file);
      while (current_char != '"') {
        add__String(&s, current_char);
        current_char = consume_char__FileIterator(&file);
      }
      token = init_string__CoulangToken(s);
      break;
    case '0' ... '9': {
      String s = init__String();
      bool is_float = false;
      while (isdigit(current_char)) {
        if (get_next_char__FileIterator(&file) == '.') {
          if (!is_float) {
            is_float = true;
          } else {
            COULANG_ERROR("the digit has to much point mother fucker")
          }
          consume_char__FileIterator(&file);
        }
        add__String(&s, current_char);
        current_char = consume_char__FileIterator(&file);
      }
      file.current_pos--;
      if (is_float) {
        token = init_float__CoulangToken(s);
      } else {
        token = init_integer__CoulangToken(s);
      }
      break;
    }
    case 'a' ... 'z': {
      String s = get_next_identifier__FileIterator(&file);
      enum CoulangTokenKind token_kind = get_keyword(s);
      if (token_kind == COULANG_TOKEN_KIND_IDENTIFIER) {
        token = init_identifier__CoulangToken(s);
      } else {
        token = init__CoulangToken(token_kind);
      }
      break;
    }
    case 'A' ... 'Z': {
      String s = get_next_identifier__FileIterator(&file);
      token = init_identifier__CoulangToken(s);
      break;
    }
    case '_': {
      String s = get_next_identifier__FileIterator(&file);
      token = init_identifier__CoulangToken(s);
      break;
    }
    case '#':
      // read til the end of the line to ignore the comments
      while (current_char == '\n') {
        current_char = consume_char__FileIterator(&file);
      }
      break;
    default:
      continue;
    }

    add__CoulangTokens(&tokens, token);
  }

  for (size_t i = 0; i < tokens.len; ++i) {
    CoulangToken current = tokens.buffer[i];

    switch (current.kind) {
    case COULANG_TOKEN_KIND_IDENTIFIER:
      printf("%d: %s\n", current.kind, current.identifier.buffer);
      break;
    case COULANG_TOKEN_KIND_FLOAT:
      printf("%d: %s\n", current.kind, current.float_.buffer);
      break;
    case COULANG_TOKEN_KIND_INTEGER:
      printf("%d: %s\n", current.kind, current.integer.buffer);
      break;
    case COULANG_TOKEN_KIND_STRING:
      printf("%d: %s\n", current.kind, current.string.buffer);
      break;
    default:
      printf("%d\n", current.kind);
    }
  }

  deinit__FileIterator(&file);
}
