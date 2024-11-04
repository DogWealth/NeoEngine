//
// Created by NeoLiu on 2024/11/2.
//

#ifndef DARRAY_H
#define DARRAY_H
#include <algorithm>
#include <iterator>
#include <vector>

#include "core/logger.h"
#include "core/memory.h"
#include "defines.h"

namespace NeoEngine {
    template<class T>
    class NEO_API DArray {
        T* data_;
        size_t size_;
        size_t capacity_;

    public:
        DArray() : data_(nullptr), size_(0), capacity_(0) {}

        explicit DArray(size_t capacity) : size_(capacity), capacity_(capacity) {
            data_ = static_cast<T *>(Memory::Allocate(capacity * sizeof(T), MemoryTag::MEMORY_TAG_DARRAY));
            for(size_t i = 0; i < size_; i++) {
                new(data_ + i) T();
            }
        }

        template<std::random_access_iterator It>
        DArray(It begin, It end) : size_(end - begin), capacity_(end - begin) {
            data_ = static_cast<T *>(Memory::Allocate(size_ * sizeof(T), MemoryTag::MEMORY_TAG_DARRAY));
            std::copy(begin, end, data_);
        }

        DArray(std::initializer_list<T> il) : DArray(il.begin(), il.end()) {

        }

        explicit DArray(size_t capacity, const T& value) : size_(capacity), capacity_(capacity) {
            data_ = static_cast<T *>(Memory::Allocate(capacity * sizeof(T), MemoryTag::MEMORY_TAG_DARRAY));
            for(size_t i = 0; i < size_; i++) {
                new(data_ + i) T(value);
            }
        }

        DArray(const DArray& other) : size_(other.size_), capacity_(other.capacity_) {
            if(size_ > 0) {
                data_ = static_cast<T *>(Memory::Allocate(capacity_ * sizeof(T), MemoryTag::MEMORY_TAG_DARRAY));
                for(size_t i = 0; i < size_; i++) {
                    new(data_ + i) T(other.data_[i]);
                }
            } else {
                data_ = nullptr;
            }
        }

        DArray(DArray&& other)  noexcept : size_(other.size_), capacity_(other.capacity_) {
            data_ = other.data_;
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }

        DArray& operator=(const DArray& other) {
            if(this != &other) {
                Clear();
                Reserve(other.capacity_);
                size_ = other.size_;
                if(size_ > 0) {
                    for(size_t i = 0; i < size_; i++) {
                        new(data_ + i) T(other.data_[i]);
                    }
                }
            }

            return *this;
        }

        DArray& operator=(DArray&& other)  noexcept {
            if(this != &other) {
                Clear();

                data_ = other.data_;
                size_ = other.size_;
                capacity_ = other.capacity_;

                other.data_ = nullptr;
                other.size_ = 0;
                other.capacity_ = 0;
            }

            return *this;
        }

        ~DArray() {
            Clear();
        }

        void Reserve(size_t capacity) {
            if(capacity > capacity_) {
                capacity = std::max(capacity, capacity_ * 2);
                auto old_data = data_;
                auto old_capacity = capacity_;

                data_ = static_cast<T *>(Memory::Allocate(capacity * sizeof(T), MemoryTag::MEMORY_TAG_DARRAY));
                capacity_ = capacity;

                for(size_t i = 0; i < size_; i++) {
                    new(data_ + i) T(old_data[i]);
                    old_data[i].~T();
                }

                Memory::Deallocate(static_cast<void *>(old_data), old_capacity * sizeof(T), MemoryTag::MEMORY_TAG_DARRAY);
            }
        }

        void Resize(size_t size) {
            Reserve(size);
            if(size > size_) {
                for(size_t i = size_; i < size; i++) {
                    new(data_ + i) T();
                }
            }
            size_ = size;
        }

        void ShrinkToFit() {
            auto old_data = data_;
            auto old_capacity = capacity_;
            capacity_ = size_;

            if(size_ == 0) {
                data_ = nullptr;
            } else {
                data_ = static_cast<T *>(Memory::Allocate(capacity_ * sizeof(T), MemoryTag::MEMORY_TAG_DARRAY));
            }

            if(old_data != nullptr) {
                if(size_ > 0) {
                    for(size_t i = 0; i < size_; i++) {
                        new(data_ + i) T(old_data[i]);
                        old_data[i].~T();
                    }
                }
                Memory::Deallocate(static_cast<void *>(old_data), old_capacity * sizeof(T), MemoryTag::MEMORY_TAG_DARRAY);
            }
        }

        void Clear() {
            Resize(0);
            ShrinkToFit();
        }

        [[nodiscard]] constexpr size_t Size() const { return size_; }

        [[nodiscard]] constexpr size_t Capacity() const { return capacity_; }

        T* Data() { return data_; }
        const T* Data() const { return data_; }

        const T& operator[](size_t i) const { return data_[i]; }
        T& operator[](size_t i) { return data_[i]; }

        T& Back() { return data_[size_ - 1]; }
        const T& Back() const { return data_[size_ - 1]; }

        void PushBack(const T& value) {
            Reserve(size_ + 1);
            new(data_ + size_) T(value);
            size_++;
        }

        void PushBack(T&& value) {
            Reserve(size_ + 1);
            new(data_ + size_) T(std::move(value));
            size_++;
        }

        void Erase(size_t i) {
            if(i < size_) {
                for(size_t j = i; j < size_ - 1; j++) {
                    data_[j].~T();
                    data_[j] = std::move(data_[j + 1]);
                }
                Resize(size_ + 1);
            } else {
                NEO_ERROR("DArray::Erase(): index out of range");
            }
        }

        void Insert(size_t i, const T& value) {
            if(i < size_) {
                Reserve(size_ + 1);
                size_++;

                for(size_t j = size_; j > i; j--) {
                    data_[j] = std::move(data_[j - 1]);
                    data_[j - 1].~T();
                }
                new(data_ + i) T(value);
            } else {
                NEO_ERROR("DArray::Insert(): index out of range");
            }
        }

        bool IsEmpty() {
            return size_ == 0;
        }
    };
}
#endif //DARRAY_H
