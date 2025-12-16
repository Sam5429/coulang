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

#ifndef COULANG_FUNCTION_H
#define COULANG_FUNCTION_H

#include <coulang/ast.h>
#include <coulang/macros.h>

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	const String *name;
	void *addr;
	enum CoulangDataType data_type;
} CoulangFunctionSymbol;

static inline CoulangFunctionSymbol
init__CoulangFunctionSymbol(const String *name, void *addr, enum CoulangDataType data_type)
{
	return (CoulangFunctionSymbol){
		.name = name,
		.addr = addr,
		.data_type = data_type
	};
}

enum CoulangFunctionKind {
	COULANG_FUNCTION_KIND_SYMBOL,
	COULANG_FUNCTION_KIND_DECL
};

typedef struct {
	enum CoulangFunctionKind kind;
	union {
		CoulangFunctionSymbol symbol;
		const CoulangDeclFunction *decl;
	};
} CoulangFunction;

CoulangFunction *
init_symbol__CoulangFunction(CoulangFunctionSymbol symbol);

CoulangFunction *
init_decl__CoulangFunction(const CoulangDeclFunction *decl);

static inline const String *
get_name__CoulangFunction(const CoulangFunction *self)
{
	switch (self->kind) {
		case COULANG_FUNCTION_KIND_SYMBOL:
			return self->symbol.name;
		case COULANG_FUNCTION_KIND_DECL:
			return self->decl->name;
		default:
			COULANG_UNREACHABLE("unknown function kind");
	}
}

static inline void
deinit__CoulangFunction(CoulangFunction *self)
{
	free(self);
}

#endif // COULANG_FUNCTION_H
