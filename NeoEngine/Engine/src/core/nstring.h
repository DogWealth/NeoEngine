//
// Created by NeoLiu on 2024/11/30.
//

#ifndef NEOENGINE_NSTRING_H
#define NEOENGINE_NSTRING_H
#include "defines.h"
namespace NeoEngine {
    NEO_API uint64_t string_len(const char* str);
    NEO_API char* string_duplicate(const char* str);
    NEO_API bool string_equal(const char* str0, const char* str1);
}


#endif //NEOENGINE_NSTRING_H
