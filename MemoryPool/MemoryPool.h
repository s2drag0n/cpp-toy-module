#pragma once

#include <cstddef>

const size_t DEFAULT_BLOCK_SIZE = 1024 * 1024;

enum class MemoryPoolError {
    CANNOT_CREATE_MEMORY_POOL,
    CONNOT_CREATE_BLOCK,
    OUT_OF_POOL,
    EXCEEDS_MAX_SIZE,
    CANNOT_CREATE_BLOCK_CHAIN
};

struct MemoryBlockHeader {
    /* memory size to allocate in this block */
    size_t block_size;
    /* point at the start of memory that has not been allocate */
    size_t offset;
    ;
    /* next block */
    MemoryBlockHeader *next;
    /* last block */
    MemoryBlockHeader *pre;
    /* number of MemoryUnit in this block, means how many times new was used */
    int num_allocated;
    int num_deleted;
};

struct MemoryUnitHeader {
    /* memory size was allocated */
    size_t length;
    /* which block this unit was allocated in */
    MemoryBlockHeader *container;
};

struct MemoryScopeHeader {
    /* start memory of this scope based on the block */
    size_t scope_offset;
    /* which block this scope was allocated */
    MemoryBlockHeader *start_block;
    MemoryScopeHeader *pre_scope;
};

class MemoryPool {
  public:
    MemoryPool(size_t block_size = DEFAULT_BLOCK_SIZE);

    ~MemoryPool();

    void *allocate(size_t size);

    template <class T> T *allocate(int instances);

    void *reallocate(void *unit_pointer_start, size_t new_size);

    template <class T>
    T *reallocate(void *unit_pointer_start, int new_instances);

    void free(void *unit_pointer_start);

    void start_scope();

    void end_scope();

    void dump_pool_data();

  private:
    size_t default_block_size;

    MemoryBlockHeader *first_block;

    MemoryBlockHeader *current_block;

    MemoryScopeHeader *current_scope;

    void create_block(size_t size);
};

template <class T> T *MemoryPool::allocate(int instances) {
    return reinterpret_cast<T *>(allocate(instances * sizeof(T)));
}

template <class T>
T *MemoryPool::reallocate(void *unit_pointer_start, int new_instances) {
    return reinterpret_cast<T *>(
        reallocate(unit_pointer_start, new_instances * sizeof(T)));
}

extern void *operator new(size_t size, MemoryPool *mp);

extern void *operator new[](size_t size, MemoryPool *mp);
