//
// Created by NeoLiu on 2024/10/31.
//

#ifndef MEMORY_H
#define MEMORY_H



namespace NeoEngine {
    enum class MemoryTag : uint8_t{
        MEMORY_TAG_UNKNOWN,
        MEMORY_TAG_ARRAY,
        MEMORY_TAG_DARRAY,
        MEMORY_TAG_DICT,
        MEMORY_TAG_RING_QUEUE,
        MEMORY_TAG_BST,
        MEMORY_TAG_STRING,
        MEMORY_TAG_APPLICATION,
        MEMORY_TAG_JOB,
        MEMORY_TAG_TEXTURE,
        MEMORY_TAG_MATERIAL_INSTANCE,
        MEMORY_TAG_RENDERER,
        MEMORY_TAG_GAME,
        MEMORY_TAG_TRANSFORM,
        MEMORY_TAG_ENTITY,
        MEMORY_TAG_ENTITY_NODE,
        MEMORY_TAG_SCENE,

        MEMORY_TAG_MAX_TAGS,
    };

    struct MemoryStates {
        uint64_t total_allocated = 0;
        uint64_t tagged_allocations[static_cast<uint8_t>(MemoryTag::MEMORY_TAG_MAX_TAGS)] = {};
    };

    class NEO_API Memory {
    public:
        Memory() = default;
        ~Memory() = default;
        static void Initialize();
        static void Destroy();

        static void* Allocate(uint64_t size, MemoryTag memory_tag);
        static void Deallocate(void* pointer, uint64_t size, MemoryTag memory_tag);
        static void* Zero_Memory(void* pointer, uint64_t size);
        static void* Copy_Memory(void* dest, const void* source, uint64_t size);
        static void* SetMemory(void* pointer, int32_t value, uint64_t size);

        static char* GetMemoryUsageStr();


    private:
        static MemoryStates states_;

    };
}



#endif //MEMORY_H
