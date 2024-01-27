#pragma once

#include <cstddef>

void *malloc_aligned(size_t size, size_t alignment);

void my_free(void *data);
