//
// Created by NeoLiu on 2024/11/30.
//

#include "nstring.h"
#include "memory.h"
#include <cstring>

namespace NeoEngine{
    uint64_t string_len(const char* str) {
        return strlen(str);
    }
    char* string_duplicate(const char* str) {
        uint64_t len = string_len(str);
        char* copy = static_cast<char*>(Memory::Allocate(len + 1, MemoryTag::MEMORY_TAG_STRING));
        Memory::Copy_Memory(copy, str, len + 1);
        return copy;
    }
}