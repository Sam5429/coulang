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

#include "coulang/ast.h"
#include <coulang/macros.h>
#include <coulang/token.h>

#include <stddef.h>
#include <stdio.h>

void deinit__CoulangToken(const CoulangToken *const self) {
  switch (self->kind) {
  case COULANG_TOKEN_KIND_IDENTIFIER:
    deinit__String(&self->identifier);

    break;
  case COULANG_TOKEN_KIND_STRING:
    deinit__String(&self->string);

    break;
  case COULANG_TOKEN_KIND_INTEGER:
    deinit__String(&self->integer);

    break;
  case COULANG_TOKEN_KIND_FLOAT:
    deinit__String(&self->float_);

    break;
  default:
    break;
  }
}

void add__CoulangTokens(CoulangTokens *self, CoulangToken token) {
  if (!self->buffer) {
    self->buffer = COULANG_ALLOC(self->capacity * sizeof(CoulangToken));
  } else if (self->len == self->capacity) {
    self->capacity *= 2;
    self->buffer =
        COULANG_REALLOC(self->buffer, self->capacity * sizeof(CoulangToken));
  }

  self->buffer[self->len++] = token;
}

void deinit__CoulangTokens(const CoulangTokens *const self) {
  for (size_t i = 0; i < self->len; i++) {
    deinit__CoulangToken(&self->buffer[i]);
  }
  free(self->buffer);
}
