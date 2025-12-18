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

#include <coulang/scope.h>

CoulangScope *
init__CoulangScope()
{
	CoulangScope *self = COULANG_ALLOC(sizeof(CoulangScope));

	*self = (CoulangScope){
		.functions = init__Map(),
		.variables = init__Map(),
		.parent = NULL
	};

	return self;
}

#define GET_T_SCOPE(map, type) \
	CoulangScope *current = self; \
	type *variable = NULL; \
\
	while (current && !variable) { \
		variable = get__Map(&(current->map), name); \
		current = current->parent; \
	} \
\
	return variable;


CoulangFunction *
get_function__CoulangScope(CoulangScope *self, const String *name)
{
	GET_T_SCOPE(functions, CoulangFunction);
}

CoulangVariable *
get_variable__CoulangScope(CoulangScope *self, const String *name)
{
	GET_T_SCOPE(variables, CoulangVariable);
}

#undef GET_T_SCOPE

void
deinit__CoulangScope(CoulangScope *self)
{
	deinit_functions__Map(&self->functions);
	deinit_variables__Map(&self->variables);
	free(self);
}
