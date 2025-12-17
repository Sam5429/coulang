#include <coulang/macros.h>
#include <coulang/string.h>

#include <stdio.h>
#include <string.h>

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

String
init_copy__String(const String *const other)
{
	String s = init__String();

	s.buffer = COULANG_ALLOC(other->capacity * sizeof(char));
	memcpy(s.buffer, other->buffer, other->capacity);

	return s;
}

String
init_from_raw__String(const char *raw)
{
	String s = init__String();

	s.len = strlen(raw);
	s.capacity = s.len + 1;
	s.buffer = COULANG_ALLOC(s.capacity * sizeof(char));
	memcpy(s.buffer, raw, s.capacity);
	s.buffer[s.len] = '\0';

	return s;
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
