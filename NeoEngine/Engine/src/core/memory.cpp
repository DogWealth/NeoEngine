//
// Created by NeoLiu on 2024/10/31.
//

#include "memory.h"

#include "nstring.h"

#include "platform/platform.h"

namespace NeoEngine {
    static const char* memory_tag_strings[static_cast<uint8_t>(MemoryTag::MEMORY_TAG_MAX_TAGS)] = {
        "UNKNOWN    ",
        "ARRAY      ",
        "DARRAY     ",
        "DICT       ",
        "RING_QUEUE ",
        "BST        ",
        "STRING     ",
        "APPLICATION",
        "JOB        ",
        "TEXTURE    ",
        "MAT_INS    ",
        "RENDERER   ",
        "GAME       ",
        "TRANSFORM  ",
        "ENTITY     ",
        "ENTITY_NODE",
        "SCENE      ",
    };

    MemoryStates Memory::states_{};

    void Memory::Initialize() {
        Platform::GetPlatform().Zero_Memory(&states_, sizeof(states_));
    }

    void Memory::Destroy() {
    }

    void * Memory::Allocate(uint64_t size, MemoryTag memory_tag) {
        if(memory_tag == MemoryTag::MEMORY_TAG_UNKNOWN) {
            NEO_WARNING("Memory::Allocate - Unknown memory tag");
        }

        states_.total_allocated += size;
        states_.tagged_allocations[static_cast<size_t>(memory_tag)] += size;

        //TODO: memory alignment
        void* ptr = Platform::GetPlatform().Allocate(size, false);
        Platform::GetPlatform().Zero_Memory(ptr, size);
        return ptr;

    }

    void Memory::Deallocate(void *pointer, uint64_t size, MemoryTag memory_tag) {
        if(memory_tag == MemoryTag::MEMORY_TAG_UNKNOWN) {
            NEO_WARNING("Memory::Deallocate - Unknown memory tag");
        }

        states_.total_allocated -= size;
        states_.tagged_allocations[static_cast<size_t>(memory_tag)] -= size;

        //TODO: memory alignment
        Platform::GetPlatform().Deallocate(pointer, false);
    }

    void * Memory::Zero_Memory(void *pointer, uint64_t size) {
        return Platform::GetPlatform().Zero_Memory(pointer, size);
    }

    void * Memory::Copy_Memory(void *dest, const void *source, uint64_t size) {
        return Platform::GetPlatform().Copy_Memory(dest, source, size);
    }

    void * Memory::SetMemory(void *pointer, int32_t value, uint64_t size) {
        return Platform::GetPlatform().SetMemory(pointer, value, size);
    }

    char* Memory::GetMemoryUsageStr() {
        const uint64_t gib = 1024 * 1024 * 1024;
        const uint64_t mib = 1024 * 1024;
        const uint64_t kib = 1024;

        char buffer[8000] = "System memory use (tagged):\n";
        uint64_t offset = string_len(buffer);
        for(uint32_t i = 0; i < static_cast<uint32_t>(MemoryTag::MEMORY_TAG_MAX_TAGS); i++) {
            char uint[4] = "XiB";
            float amount = 1.0f;
            if(states_.tagged_allocations[i] >= gib) {
                uint[0] = 'G';
                amount = states_.tagged_allocations[i] / static_cast<float>(gib);
            } else if (states_.tagged_allocations[i] >= mib) {
                uint[0] = 'M';
                amount = states_.tagged_allocations[i] / static_cast<float>(mib);
            } else if (states_.tagged_allocations[i] >= kib) {
                uint[0] = 'K';
                amount = states_.tagged_allocations[i] / static_cast<float>(kib);
            } else {
                uint[0] = 'B';
                uint[1] = '\0';
                amount = static_cast<float>(states_.tagged_allocations[i]);
            }
            uint32_t length = snprintf(buffer + offset, 8000, "%s: %.2f%s\n", memory_tag_strings[i], amount, uint);
            offset += length;
        }

        return string_duplicate(buffer);
    }
}
