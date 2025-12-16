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
	COULANG_VALUE_KIND_INT,
	COULANG_VALUE_KIND_FLOAT,
	COULANG_VALUE_KIND_LIST,
	COULANG_VALUE_KIND_PTR,
	COULANG_VALUE_KIND_STR,
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

static inline CoulangValue
init_int__CoulangValue(int64_t int_)
{
	return (CoulangValue){
		.kind = COULANG_VALUE_KIND_INT,
		.int_ = int_
	};
}

static inline CoulangValue
init_float__CoulangValue(double float_)
{
	return (CoulangValue){
		.kind = COULANG_VALUE_KIND_FLOAT,
		.float_ = float_
	};
}

static inline CoulangValue
init_list__CoulangValue(CoulangValueList list)
{
	return (CoulangValue){
		.kind = COULANG_VALUE_KIND_LIST,
		.list = list
	};
}

static inline CoulangValue
init_ptr__CoulangValue(void *ptr)
{
	return (CoulangValue){
		.kind = COULANG_VALUE_KIND_PTR,
		.ptr = ptr
	};
}

static inline CoulangValue
init_str__CoulangValue(const String *str)
{
	return (CoulangValue){
		.kind = COULANG_VALUE_KIND_STR,
		.str = str
	};
}

void
deinit__CoulangValue(const CoulangValue *const self);

#endif // COULANG_VALUE_H
