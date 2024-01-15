#include "MemoryPool.h"
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <ostream>

MemoryPool::MemoryPool(size_t block_size)
    : default_block_size(block_size), first_block(nullptr),
      current_block(nullptr), current_scope(nullptr) {
    create_block(default_block_size);
}

MemoryPool::~MemoryPool() {
    while (first_block != nullptr) {
        MemoryBlockHeader *temp = first_block->next;
        std::free(first_block);
        first_block = temp;
    }
}

void MemoryPool::create_block(size_t size) {

    MemoryBlockHeader *new_block = reinterpret_cast<MemoryBlockHeader *>(
        malloc(sizeof(MemoryBlockHeader) + size));
    if (new_block == nullptr) {
        throw MemoryPoolError::CONNOT_CREATE_BLOCK;
    }

    new_block->pre = current_block;
    new_block->next = nullptr;
    new_block->block_size = size;
    new_block->offset = 0;
    new_block->num_allocated = 0;
    new_block->num_deleted = 0;

    if (current_block) {
        current_block->next = new_block;
    } else {
        first_block = new_block;
    }

    current_block = new_block;
}

void *MemoryPool::allocate(size_t size) {

    /* size needed cannot be store in a default block */
    if (sizeof(MemoryUnitHeader) + size > default_block_size) {
        create_block(sizeof(MemoryUnitHeader) + size);
    } /* not enough space in this block, crate a new one */
    else if (sizeof(MemoryUnitHeader) + size > current_block->block_size -
                                                   sizeof(MemoryBlockHeader) -
                                                   current_block->offset) {
        create_block(default_block_size);
    }

    /* now enough space in this block */
    MemoryUnitHeader *unit = reinterpret_cast<MemoryUnitHeader *>(
        reinterpret_cast<char *>(current_block) + sizeof(MemoryBlockHeader) +
        current_block->offset);

    unit->length = size;
    unit->container = current_block;

    current_block->num_allocated++;
    current_block->offset += sizeof(MemoryUnitHeader) + size;

    return reinterpret_cast<void *>(unit + 1);
}

void *MemoryPool::reallocate(void *unit_pointer_start, size_t new_size) {

    /* get unit & block */
    MemoryUnitHeader *target_unit = reinterpret_cast<MemoryUnitHeader *>(
        reinterpret_cast<char *>(unit_pointer_start) -
        sizeof(MemoryUnitHeader));

    MemoryBlockHeader *target_block = target_unit->container;

    /* if this unit is the last one and enough space in this block, reset length
     */
    if (reinterpret_cast<char *>(unit_pointer_start) + target_unit->length ==
            reinterpret_cast<char *>(target_block + 1) + target_block->offset &&
        target_block->block_size - target_block->offset >=
            new_size - target_unit->length) {

        target_block->offset += new_size - target_unit->length;
        target_unit->length = new_size;

        return unit_pointer_start;
    }

    /* delete this unit and create a new one */
    free(unit_pointer_start);
    void *temp = allocate(new_size);
    memcpy(temp, unit_pointer_start, target_unit->length);

    return temp;
}

void MemoryPool::free(void *unit_pointer_start) {

    /* get unit & block */
    MemoryUnitHeader *target_unit = reinterpret_cast<MemoryUnitHeader *>(
        reinterpret_cast<char *>(unit_pointer_start) -
        sizeof(MemoryUnitHeader));

    MemoryBlockHeader *target_block = target_unit->container;

    /* this unit is the last one, reset offset */
    if (reinterpret_cast<char *>(unit_pointer_start) + target_unit->length ==
        reinterpret_cast<char *>(target_block + 1) + target_block->offset) {

        target_block->offset = reinterpret_cast<char *>(target_unit) -
                               reinterpret_cast<char *>(target_block + 1);

        target_block->num_allocated--;
    } else {
        target_block->num_deleted++;
    }

    /* if block has no unit, delete it */
    if (target_block->offset == 0 ||
        target_block->num_deleted == target_block->num_allocated) {

        /* if only this block exist */
        if (target_block == first_block) {
            first_block->offset = 0;
        } else {
            if (target_block == current_block) {
                current_block = target_block->pre;
            } else {
                target_block->pre->next = target_block->next;
                target_block->next->pre = target_block->pre;
            }
            std::free(target_block);
        }
    }
}

void MemoryPool::start_scope() {

    MemoryScopeHeader *new_scope = new (this) struct MemoryScopeHeader;
    new_scope->start_block = current_block;
    new_scope->scope_offset = current_block->offset - sizeof(MemoryUnitHeader) -
                              sizeof(MemoryScopeHeader);

    new_scope->pre_scope = current_scope;
    current_scope = new_scope;
}

void MemoryPool::end_scope() {
    if (current_scope == nullptr) {
        return;
    }

    while (current_block != current_scope->start_block) {
        MemoryBlockHeader *temp = current_block->pre;
        std::free(current_block);
        current_block = temp;
    }

    MemoryScopeHeader *temp = current_scope->pre_scope;

    current_block->offset = current_scope->scope_offset;
    if (current_block != first_block && current_block->offset == 0) {
        MemoryBlockHeader *temp = current_block;
        std::free(current_block);
        current_block = temp;
    }

    current_scope = temp;
}

void MemoryPool::dump_pool_data() {
    MemoryBlockHeader *block = first_block;

    int num_block = 1;
    while (block != nullptr) {

        std::cout << "Block " << num_block << ": " << std::endl;
        std::cout << "\tUsed: "
                  << static_cast<float>(block->offset) /
                         static_cast<float>(block->block_size) * 100
                  << "%( " << block->offset << "/" << block->block_size << " )"
                  << std::endl;

        size_t unit_offset = 0;
        int num_unit = 1;
        std::cout << "Units: ===============" << std::endl;
        while (unit_offset + sizeof(MemoryBlockHeader) <= block->offset) {
            MemoryUnitHeader *cur_unit = reinterpret_cast<MemoryUnitHeader *>(
                reinterpret_cast<char *>(block + 1) + unit_offset);
            std::cout << "\t\tUnit " << num_unit << ": "
                      << cur_unit->length + sizeof(MemoryUnitHeader)
                      << std::endl;

            unit_offset += cur_unit->length + sizeof(MemoryUnitHeader);
            num_unit++;
        }
        std::cout << "======================" << std::endl;

        block = block->next;
        num_block++;
    }
}

void *operator new(size_t _size, MemoryPool *mp) { return mp->allocate(_size); }

void *operator new[](size_t _size, MemoryPool *mp) {
    return mp->allocate(_size);
}
