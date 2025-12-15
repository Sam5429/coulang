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

#ifndef COULANG_TOKEN_H
#define COULANG_TOKEN_H

#include <coulang/string.h>

enum CoulangTokenKind {
	COULANG_TOKEN_KIND_PLUS,
	COULANG_TOKEN_KIND_MINUS,
	COULANG_TOKEN_KIND_STAR,
	COULANG_TOKEN_KIND_SLASH,
	COULANG_TOKEN_KIND_MODULO,
	COULANG_TOKEN_KIND_LPAREN,
	COULANG_TOKEN_KIND_RPAREN,
	COULANG_TOKEN_KIND_LBRACE,
	COULANG_TOKEN_KIND_RBRACE,
	COULANG_TOKEN_KIND_STRING,
	COULANG_TOKEN_KIND_EQ,
	COULANG_TOKEN_KIND_EQ_EQ,
	COULANG_TOKEN_KIND_BANG,
	COULANG_TOKEN_KIND_BANG_EQ,
	COULANG_TOKEN_KIND_LESS,
	COULANG_TOKEN_KIND_GREATER,
	COULANG_TOKEN_KIND_LESS_EQ,
	COULANG_TOKEN_KIND_GREATER_EQ,
	COULANG_TOKEN_KIND_INTEGER,
	COULANG_TOKEN_KIND_FLOAT,
	COULANG_TOKEN_KIND_IDENTIFIER,
	COULANG_TOKEN_KIND_KEYWORD_FN,
	COULANG_TOKEN_KIND_KEYWORD_RETURN,
	COULANG_TOKEN_KIND_KEYWORD_LOAD,
	COULANG_TOKEN_KIND_KEYWORD_INT,
	COULANG_TOKEN_KIND_KEYWORD_FLOAT,
	COULANG_TOKEN_KIND_KEYWORD_STR,
	COULANG_TOKEN_KIND_KEYWORD_LIST,
};

typedef struct {
	enum CoulangTokenKind kind;
	union {
		String identifier;
		String string;
		String integer;
		String float_;
	};
} CoulangToken;

static inline CoulangToken
init__CoulangToken(enum CoulangTokenKind kind)
{

	return (CoulangToken){
		.kind = kind
	};
}

static inline CoulangToken
init_identifier__CoulangToken(String identifier)
{
	return (CoulangToken){
		.kind = COULANG_TOKEN_KIND_IDENTIFIER,
		.identifier = identifier
	};
}

static inline CoulangToken
init_string__CoulangToken(String string)
{
	return (CoulangToken){
		.kind = COULANG_TOKEN_KIND_STRING,
		.string = string
	};
}

static inline CoulangToken
init_integer__CoulangToken(String integer)
{
	return (CoulangToken){
		.kind = COULANG_TOKEN_KIND_INTEGER,
		.integer = integer
	};
}

static inline CoulangToken
init_float__CoulangToken(String float_)
{
	return (CoulangToken){
		.kind = COULANG_TOKEN_KIND_FLOAT,
		.float_ = float_
	};
}

void
deinit__CoulangToken(const CoulangToken *const self);

#endif // COULANG_TOKEN_H
