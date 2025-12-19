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
#include <coulang/string.h>

#include <stdio.h>
#include <string.h>

void add__String(String *self, char c) {
  if (!self->buffer) {
    self->buffer = COULANG_ALLOC(self->capacity * sizeof(char));
  } else if (self->len + 1 == self->capacity) {
    self->capacity *= 2;
    self->buffer = COULANG_REALLOC(self->buffer, self->capacity * sizeof(char));
  }

  self->buffer[self->len++] = c;
  self->buffer[self->len] = '\0';
}

String init_copy__String(const String *const other) {
  String s = init__String();

  s.buffer = COULANG_ALLOC(other->capacity * sizeof(char));
  memcpy(s.buffer, other->buffer, other->capacity);

  return s;
}

String init_from_raw__String(const char *raw) {
  String s = init__String();

  s.len = strlen(raw);
  s.capacity = s.len + 1;
  s.buffer = COULANG_ALLOC(s.capacity * sizeof(char));
  memcpy(s.buffer, raw, s.capacity);
  s.buffer[s.len] = '\0';

  return s;
}

String init_from_2_row__String(const char *raw1, const char *raw2) {
  String s = init__String();

  s.len = strlen(raw1) + strlen(raw2);
  s.capacity = s.len + 1;
  s.buffer = COULANG_ALLOC(s.capacity * sizeof(char));
  memcpy(s.buffer, raw1, strlen(raw1));
  memcpy(s.buffer + strlen(raw1), raw2, strlen(raw2));
  s.buffer[s.len] = '\0';

  return s;
}

void add__Strings(Strings *self, String s) {
  if (!self->buffer) {
    self->buffer = COULANG_ALLOC(self->capacity * sizeof(String));
  } else if (self->len == self->capacity) {
    self->capacity *= 2;
    self->buffer =
        COULANG_REALLOC(self->buffer, self->capacity * sizeof(String));
  }

  self->buffer[self->len++] = s;
}
