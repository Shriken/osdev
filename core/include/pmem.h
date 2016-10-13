#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define PMEM_BLOCK_SIZE 4096

// TODO: Make this multiprocess safe

void pmem_init(int64_t memSize, uint32_t* bitmap);
void pmem_init_region(void* base, int64_t size);
void pmem_deinit_region(void* base, int64_t size);

void* pmem_alloc_block();
void* pmem_alloc_blocks(uint32_t num_blocks);
void pmem_free_block(void* block);
void pmem_free_blocks(void* block, uint32_t num_blocks);
