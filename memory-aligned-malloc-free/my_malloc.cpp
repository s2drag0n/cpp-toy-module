#include "my_malloc.h"
#include <cstdlib>

void *malloc_aligned(size_t size, size_t alignment) {
    size_t offset = alignment - 1 + sizeof(void *);
    void *real_start = malloc(size + offset);
    void *start = (void *)(((size_t)real_start + offset) & ~(alignment - 1));
    void **real_start_recorder = (void **)((size_t)start - sizeof(void *));
    *real_start_recorder = real_start;
    return start;
}

void my_free(void *data) {
    void **real_start_recorder = (void **)((size_t)data - sizeof(void *));
    free(*real_start_recorder);
}
