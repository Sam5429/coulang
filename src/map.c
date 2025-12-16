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

#include <coulang/map.h>
#include <coulang/macros.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline MapPair
init__MapPair(const String *key, void *value);

static MapBucket *
init__MapBucket(MapPair pair);

static void
deinit__MapBucket(MapBucket *self);

static const MapBucket *
get_bucket__MapBucket(MapBucket *self, const String *key);

static void *
push_bucket__Map(Map *self, size_t index, MapBucket *new);

static size_t
hash__Map(const String *key);

MapPair
init__MapPair(const String *key, void *value)
{
	return (MapPair){
		.key = key,
		.value = value
	};
}

MapBucket *
init__MapBucket(MapPair pair)
{
	MapBucket *self = COULANG_ALLOC(sizeof(MapBucket));

	*self = (MapBucket){
		.pair = pair,
		.next = NULL
	};

	return self;
}

void
deinit__MapBucket(MapBucket *self)
{
	MapBucket *current = self;

	while (current) {
		MapBucket *next = current->next;

		free(current);

		current = next;
	}
}

const MapBucket *
get_bucket__MapBucket(MapBucket *self, const String *key)
{
	if (!strcmp(self->pair.key->buffer, key->buffer)) {
		return self;
	}

	return self->next ? get_bucket__MapBucket(self->next, key) : NULL;
}

void *
get__Map(Map *self, const String *key)
{
#define INDEX(key) hash__Map((key)) % self->capacity
	if (!self->buckets) {
		return NULL;
	}

	size_t index = INDEX(key);
	MapBucket *bucket = self->buckets[index];

	if (bucket) {
		const MapBucket *res = get_bucket__MapBucket(bucket, key);

		return res ? res->pair.value : NULL;
	}

	return NULL;
}

void *
push_bucket__Map(Map *self, size_t index, MapBucket *new)
{
	void *is_exist = get__Map(self, new->pair.key);

	if (is_exist) {
		deinit__MapBucket(new);

		return is_exist;
	}

	if (self->buckets[index]) {
		MapBucket *current = self->buckets[index];

		while (current->next) {
			current = current->next;
		}

		current->next = new;

		return NULL;
	}

	self->buckets[index] = new;

	return NULL;
}

size_t
hash__Map(const String *key)
{
	// https://en.wikipedia.org/wiki/Jenkins_hash_function
	size_t len = strlen(key->buffer);
	size_t hash = 0;

	for (size_t i = 0; i < len; ++i) {
		hash += key->buffer[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}

	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);

	return hash;
}

void *
insert__Map(Map *self, const String *key, void *value)
{
	size_t index = INDEX(key);

	if (!self->buckets) {
		self->buckets = COULANG_ARRAY_ALLOC(DEFAULT_MAP_CAPACITY, sizeof(void*));
		self->buckets[index] = init__MapBucket(init__MapPair(key, value));
		++self->len;

		return NULL;
	}

	if (self->len + 1 > self->capacity) {
		self->capacity *= 2;

		MapBucket **new_buckets = COULANG_ARRAY_ALLOC(self->capacity, sizeof(void*));
		MapBucket **old_buckets = self->buckets;

		self->buckets = new_buckets;
		
		for (size_t i = 0; i < self->len; ++i) {
			MapBucket *current = old_buckets[i];

			while (current) {
				MapBucket *next = current->next;
				size_t new_index = INDEX(current->pair.key);

				current->next = new_buckets[new_index];
				new_buckets[new_index] = current;
				current = next;
			}
		}

		free(old_buckets);

		index = INDEX(key);
	}

	void *is_exist = push_bucket__Map(self, index, init__MapBucket(init__MapPair(key, value)));

	if (is_exist) {
		return is_exist;
	}

	++self->len;

	return NULL;

#undef INDEX
}

void
deinit__Map(const Map *const self)
{
	for (size_t i = 0; i < self->len; ++i) {
		deinit__MapBucket(self->buckets[i]);
	}

	free(self->buckets);
}
