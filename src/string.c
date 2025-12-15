#include <coulang/macros.h>
#include <coulang/string.h>

#include <stdio.h>

void add__String(String *self, char c) {
  if (!self->buffer) {
    self->buffer = COULANG_ALLOC(self->capacity * sizeof(char));
  } else if (self->len + 1 == self->capacity) {
    self->capacity *= 2;
    self->buffer = COULANG_REALLOC(self->buffer, self->capacity * sizeof(char));
  }

  self->buffer[self->len++] = c;
  self->buffer[self->len] = '\0';
}

void
add__Strings(Strings *self, String s)
{
	if (!self->buffer) {
		self->buffer = COULANG_ALLOC(self->capacity * sizeof(String));
	} else if (self->len == self->capacity) {
		self->capacity *= 2;
		self->buffer = COULANG_REALLOC(self->buffer, self->capacity * sizeof(String));
	}

	self->buffer[self->len++] = s;
}
