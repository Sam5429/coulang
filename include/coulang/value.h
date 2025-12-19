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

#ifndef COULANG_VALUE_H
#define COULANG_VALUE_H

#include <coulang/string.h>

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

typedef struct CoulangValue CoulangValue;

typedef struct {
	CoulangValue *buffer;
	size_t len;
} CoulangValueList;

static inline CoulangValueList
init__CoulangValueList(CoulangValue *buffer, size_t len)
{
	return (CoulangValueList){
		.buffer = buffer,
		.len = len
	};
}

static inline void
deinit__CoulangValueList(const CoulangValueList *const self)
{
	free(self->buffer);
}

enum CoulangValueKind {
	COULANG_VALUE_KIND_INT = 1 << 1,
	COULANG_VALUE_KIND_FLOAT = 1 << 2,
	COULANG_VALUE_KIND_LIST = 1 << 3,
	COULANG_VALUE_KIND_PTR = 1 << 4,
	COULANG_VALUE_KIND_STR = 1 << 5,
};

typedef struct CoulangValue {
	enum CoulangValueKind kind;
	union {
		int64_t int_;
		double float_;
		CoulangValueList list;
		void *ptr;
		const String *str;
	};
} CoulangValue;

CoulangValue
init_int__CoulangValue(int64_t int_);

CoulangValue
init_float__CoulangValue(double float_);

CoulangValue
init_list__CoulangValue(CoulangValueList list);

CoulangValue
init_ptr__CoulangValue(void *ptr);

CoulangValue
init_str__CoulangValue(const String *str);

static inline bool
is_initialized__CoulangValue(const CoulangValue *self)
{
	return self->kind != 0;
}

static inline bool
is_cond_true__CoulangValue(CoulangValue *self)
{
	if (self->kind == COULANG_VALUE_KIND_INT) {
		return self->int_ != 0;
	} else {
		COULANG_INTERPRETER_ERROR("expected integer type on condition");
	}
}

bool
is_integer__CoulangValue(CoulangValue *self);

uintptr_t
get_integer_as_c_value__CoulangValue(CoulangValue *self);

double
get_float_as_c_value__CoulangValue(CoulangValue *self);

void
deinit__CoulangValue(const CoulangValue *const self);

#endif // COULANG_VALUE_H
