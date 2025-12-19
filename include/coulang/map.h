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

#ifndef COULANG_MAP_H
#define COULANG_MAP_H

#include <coulang/string.h>

#include <stddef.h>

typedef struct {
	const String *key;
	void *value;
} MapPair;

typedef struct MapBucket {
	MapPair pair;
	struct MapBucket *next;
} MapBucket;

#define DEFAULT_MAP_CAPACITY 8

typedef struct {
	MapBucket **buckets;
	size_t len;
	size_t capacity;
} Map;

static inline Map
init__Map()
{
	return (Map){
		.buckets = NULL,
		.len = 0,
		.capacity = DEFAULT_MAP_CAPACITY
	};
}

void *
get__Map(Map *self, const String *key);

void *
insert__Map(Map *self, const String *key, void *value);

void
deinit__Map(const Map *const self);

void
deinit_functions__Map(const Map *const self);

void
deinit_variables__Map(const Map *const self);

void
deinit_lib_handle__Map(const Map *const self);

#endif // COULANG_MAP_H
