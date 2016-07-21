#pragma once

#include "size_t.h"
#include "stdint.h"

size_t strlen(const char* str);
void* memcpy(void* dest, void* src, size_t n);
void* memset(void* s, uint8_t x, size_t n);
