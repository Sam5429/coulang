#ifndef COULANG_STRING_H
#define COULANG_STRING_H

#include <stdlib.h>

#define DEFAULT_STRING_CAPACITY 8

typedef struct {
	char *buffer;
	size_t len;
	size_t capacity;
} String;

static inline String
init__String()
{
	return (String){
		.buffer = NULL,
		.len = 0,
		.capacity = DEFAULT_STRING_CAPACITY
	};
}

void
add__String(String *self, char c);

static inline void
deinit__String(const String *const self)
{
	free(self->buffer);
}

#endif // COULANG_STRING_H
