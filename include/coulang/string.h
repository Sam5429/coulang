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
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef COULANG_STRING_H
#define COULANG_STRING_H

#include <stdlib.h>

#define STRING_DEFAULT_CAPACITY 8

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
		.capacity = STRING_DEFAULT_CAPACITY 
	};
}

String
init_copy__String(const String *const other);

String
init_from_raw__String(const char *raw);

void
add__String(String *self, char c);

static inline void
deinit__String(const String *const self)
{
	free(self->buffer);
}

#define STRINGS_DEFAULT_CAPACITY 8

typedef struct {
	String *buffer;
	size_t len;
	size_t capacity;
} Strings;

static inline Strings
init__Strings()
{
	return (Strings){
		.buffer = NULL,
		.len = 0,
		.capacity = STRINGS_DEFAULT_CAPACITY
	};
}

void
add__Strings(Strings *self, String s);

static inline void
deinit__Strings(const Strings *const self)
{
	for (size_t i = 0; i < self->len; ++i) {
		deinit__String(&self->buffer[i]);
	}

	free(self->buffer);
}

#endif // COULANG_STRING_H
