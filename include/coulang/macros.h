#ifndef COULANG_MACROS_H
#define COULANG_MACROS_H

#define COULANG_ERROR(message, ...)                                            \
  fprintf(stderr, "Error(%s:%d): " message "\n", __FILE__, __LINE__,           \
          ##__VA_ARGS__);                                                      \
  exit(1);

#define COULANG_INTERPRETER_ERROR(message, ...)                                \
  fprintf(stderr, "Error:" message "\n", ##__VA_ARGS__);                       \
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

#define COULANG_ZERO_ALLOC(size) COULANG_F_ALLOC(calloc, 1, size);

#define COULANG_REALLOC(ptr, size) COULANG_F_ALLOC(realloc, ptr, size);

#endif // COULANG_MACROS_H
