#pragma once

#include <gfx/hal/MemoryAllocator.h>

namespace gfx {

class GLMemoryAllocator : public MemoryAllocator {
public:
    GLMemoryAllocator();
    ~GLMemoryAllocator() override;
    
    MemoryAllocation allocate(u64 size, u32 memoryTypeBits, const AllocationFlag& flags = {}) override;
    void free(const MemoryAllocation& allocation) override;
    void* map(const MemoryAllocation& allocation) override;
    void unmap(const MemoryAllocation& allocation) override;
    void flush(const MemoryAllocation& allocation, u64 offset = 0, u64 size = ~0ull) override;
    void invalidate(const MemoryAllocation& allocation, u64 offset = 0, u64 size = ~0ull) override;
    u32 findMemoryType(u32 typeFilter, MemoryType properties) override;
};

}