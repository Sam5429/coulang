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

#ifndef COULANG_SCOPE_H
#define COULANG_SCOPE_H

#include <coulang/ast.h>
#include <coulang/map.h>
#include <coulang/macros.h>
#include <coulang/variable.h>

#include <stdio.h>

typedef struct {
	Map functions;
	Map variables;
} CoulangScope;

static inline CoulangScope
init__CoulangScope()
{
	return (CoulangScope){
		.functions = init__Map(),
		.variables = init__Map(),
	};
}

static inline void
add_function__CoulangScope(CoulangScope *self, CoulangDeclFunction *function)
{
	if (insert__Map(&self->functions, function->name, function)) {
		COULANG_INTERPRETER_ERROR("duplicate function name");
	}
}

static inline void
add_variable__CoulangScope(CoulangScope *self, CoulangVariable *variable)
{
	if (insert__Map(&self->variables, variable->name, variable)) {
		COULANG_INTERPRETER_ERROR("duplicate variable name");
	}
}

static inline CoulangDeclFunction *
get_function__CoulangScope(CoulangScope *self, const String *name)
{
	return get__Map(&self->functions, name);
}

static inline CoulangVariable *
get_variable__CoulangScope(CoulangScope *self, const String *name)
{
	return get__Map(&self->variables, name);
}

void
deinit__CoulangScope(CoulangScope *self);

#endif // COULANG_SCOPE_H
