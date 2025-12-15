#include <coulang/macros.h>
#include <coulang/string.h>

#include <stdio.h>

void
add__String(String *self, char c)
{
	if (!self->buffer) {
		self->buffer = COULANG_ALLOC(DEFAULT_STRING_CAPACITY * sizeof(char));
	} else if (self->len + 1 == self->capacity) {
		self->capacity *= 2;
		self->buffer = COULANG_REALLOC(self->buffer, self->capacity * sizeof(char));
	}

	self->buffer[self->len++] = c;
	self->buffer[self->len] = '\0';
}
