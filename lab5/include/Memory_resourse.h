#pragma once
#include <memory_resource>
#include <map>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <memory>

using namespace std;

class MemoryResource : public pmr::memory_resource {
private:
    void* bufferStart_;
    size_t bufferSize_;
    size_t currentOffset_;

    map<void*, size_t> allocatedBlocks_;
    
    map<size_t, vector<void*>> freeBlocks_;

public:
    MemoryResource(size_t size_bytes) 
        : bufferSize_(size_bytes), currentOffset_(0) {
        bufferStart_ = ::operator new(size_bytes);
    }

    ~MemoryResource() {
        ::operator delete(bufferStart_);
    }

protected:
    void* do_allocate(size_t bytes, size_t alignment) override {
        auto it = freeBlocks_.lower_bound(bytes);
        if (it != freeBlocks_.end() && !it->second.empty()) {
            void* ptr = it->second.back();
            it->second.pop_back();
            if (it->second.empty()) {
                freeBlocks_.erase(it);
            }
            allocatedBlocks_[ptr] = bytes;
            return ptr;
        }

        void* currentPtr = static_cast<char*>(bufferStart_) + currentOffset_;
        size_t space = bufferSize_ - currentOffset_;

        void* alignedPtr = currentPtr;
        if (align(alignment, bytes, alignedPtr, space)) {
            size_t diff = static_cast<char*>(alignedPtr) - static_cast<char*>(currentPtr);
            currentOffset_ += diff + bytes;
            
            allocatedBlocks_[alignedPtr] = bytes;
            return alignedPtr;
        }

        throw bad_alloc();
    }

    void do_deallocate(void* p, size_t bytes, size_t alignment) override {
        auto it = allocatedBlocks_.find(p);
        if (it != allocatedBlocks_.end()) {
            allocatedBlocks_.erase(it);
            freeBlocks_[bytes].push_back(p);
        }
    }

    bool do_is_equal(const memory_resource& other) const noexcept override {
        return this == &other;
    }
};