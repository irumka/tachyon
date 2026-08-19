#pragma once

#include <cstddef>
#include <cstdlib>
#include <new>
#include <stdexcept>

namespace tachyon::memory {

class PoolAllocator {
private:
    void* mem_ptr{nullptr};
    void* free_list_head{nullptr};

public:
    PoolAllocator(size_t count, size_t chunk_size) {
        if (count == 0 || chunk_size < sizeof(void*)) {
            throw std::invalid_argument("PoolAllocator: count must be > 0 and chunk_size >= sizeof(void*)");
        }

        mem_ptr = std::malloc(count * chunk_size);
        if (!mem_ptr) {
            throw std::bad_alloc();
        }

        char* current = static_cast<char*>(mem_ptr);

        for (size_t i = 0; i < count - 1; ++i) {
            char* next = current + chunk_size;

            *reinterpret_cast<void**>(current) = next;

            current = next;
        }

        *reinterpret_cast<void**>(current) = nullptr;

        free_list_head = mem_ptr;
    }

    ~PoolAllocator() {
        std::free(mem_ptr);
    }

    PoolAllocator(const PoolAllocator&) = delete;
    PoolAllocator& operator=(const PoolAllocator&) = delete;

    void* allocate() {
        if (!free_list_head) {
            throw std::bad_alloc();
        }

        void* ptr = free_list_head;
        free_list_head = *reinterpret_cast<void**>(free_list_head);
        return ptr;
    }

    void deallocate(void* ptr) {
        if (!ptr) return;

        *reinterpret_cast<void**>(ptr) = free_list_head;
        free_list_head = ptr;
    }
};

}