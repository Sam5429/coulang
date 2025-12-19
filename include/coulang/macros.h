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

#ifndef COULANG_MACROS_H
#define COULANG_MACROS_H

#include <stdio.h>
#include <stdlib.h>

#define COULANG_ERROR(message, ...)                                            \
  fprintf(stderr, "Error(%s:%d): " message "\n", __FILE__, __LINE__,           \
          ##__VA_ARGS__);                                                      \
  exit(1);

#define COULANG_UNREACHABLE(message)                                           \
  fprintf(stderr, "Unreachable(%s:%d): " message "\n", __FILE__, __LINE__);    \
  exit(1);

#define COULANG_INTERPRETER_ERROR(message, ...)                                \
  fprintf(stderr, "Error: " message "\n", ##__VA_ARGS__);                      \
  exit(1);

#define COULANG_F_ALLOC(f, ...)                                                \
  ({                                                                           \
    void *_mem = f(__VA_ARGS__);                                               \
                                                                               \
    if (!_mem) {                                                               \
      COULANG_ERROR("not enough memory");                                      \
    }                                                                          \
                                                                               \
    _mem;                                                                      \
  })

#define COULANG_ALLOC(size) COULANG_F_ALLOC(malloc, size)

#define COULANG_ARRAY_ALLOC(n, size) COULANG_F_ALLOC(calloc, n, size);

#define COULANG_ZERO_ALLOC(size) COULANG_ARRAY_ALLOC(1, size);

#define COULANG_REALLOC(ptr, size) COULANG_F_ALLOC(realloc, ptr, size);

#endif // COULANG_MACROS_H
