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

#include <coulang/macros.h>
#include <coulang/value.h>

#include <stdio.h>

CoulangValue
init_int__CoulangValue(int64_t int_)
{
	return (CoulangValue){
		.kind = COULANG_VALUE_KIND_INT,
		.int_ = int_
	};
}

CoulangValue
init_float__CoulangValue(double float_)
{
	return (CoulangValue){
		.kind = COULANG_VALUE_KIND_FLOAT,
		.float_ = float_
	};
}

CoulangValue
init_list__CoulangValue(CoulangValueList list)
{
	return (CoulangValue){
		.kind = COULANG_VALUE_KIND_LIST,
		.list = list
	};
}

CoulangValue
init_ptr__CoulangValue(void *ptr)
{
	return (CoulangValue){
		.kind = COULANG_VALUE_KIND_PTR,
		.ptr = ptr
	};
}

CoulangValue
init_str__CoulangValue(const String *str)
{
	return (CoulangValue){
		.kind = COULANG_VALUE_KIND_STR,
		.str = str
	};
}

bool
is_integer__CoulangValue(CoulangValue *self)
{
	return self->kind != COULANG_VALUE_KIND_FLOAT;
}

uintptr_t
get_integer_as_c_value__CoulangValue(CoulangValue *self)
{
	switch (self->kind) {
		case COULANG_VALUE_KIND_INT:
			return self->int_;
		case COULANG_VALUE_KIND_PTR:
			return (uintptr_t)self->ptr;
		case COULANG_VALUE_KIND_STR:
			return (uintptr_t)self->str->buffer;
		default:
			COULANG_INTERPRETER_ERROR("this is not a C compatible integer");
	}
}

double
get_float_as_c_value__CoulangValue(CoulangValue *self)
{
	switch (self->kind) {
		case COULANG_VALUE_KIND_FLOAT:
			return self->float_;
		default:
			COULANG_INTERPRETER_ERROR("this is not a C compatible float");
	}
}

void
deinit__CoulangValue(const CoulangValue *const self)
{
	switch (self->kind) {
		case COULANG_VALUE_KIND_LIST:
			deinit__CoulangValueList(&self->list);

			break;
		default:
			break;
	}
}
