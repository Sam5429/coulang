#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

void err(char *error) {
  printf("%s\n", error);
  exit(-1);
}

typedef struct file_iterator {
  char *content;
  size_t current_pos;
  size_t size;
} file_iterator;

file_iterator read_file(char *path) {
  FILE *f = fopen(path, "r");
  if (f == NULL) {
    err("error while opening the file");
  }

  struct stat s;
  if (stat(path, &s) == -1) {
    err("error while reading the size of the file");
  }

  char *buffer = malloc(s.st_size + 1);

  if (fread(buffer, 1, s.st_size, f) != s.st_size) {
    err("error while reading the file");
  }
  buffer[s.st_size] = '\0';

  fclose(f);

  return (file_iterator){
      .content = buffer,
      .current_pos = 0,
      .size = s.st_size,
  };
}

int is_done(file_iterator *file) {
  if (file->current_pos != file->size - 1) {
    return 0;
  }
  return 1;
}

/*
 * return -1 if the file is done
 */
char consume_char(file_iterator *file) {
  if (!is_done(file)) {
    return file->content[file->current_pos++];
  }
  return -1;
}

void destroy(file_iterator *file) { free(file->content); }

void lex(char *path) {
  file_iterator file = read_file(path);
  while (!is_done(&file)) {
    printf("%c", consume_char(&file));
  }
  destroy(&file);
}
