#pragma once

#include <gfx/core/types.h>

namespace gfx {

enum class MemoryType {
    DEVICE_LOCAL = 1,
    HOST_VISIBLE = 2,
    HOST_COHERENT = 4,
    HOST_CACHED = 8,
    LAZILY_ALLOCATED = 16
};

enum class AllocationFlag : u32 {
    NONE = 0,
    MAPPED = 1,
    CAN_ALIAS = 2,
};

inline AllocationFlag operator|(AllocationFlag a, AllocationFlag b) {
    return static_cast<AllocationFlag>(static_cast<u32>(a) | static_cast<u32>(b));
}

inline AllocationFlag operator&(AllocationFlag a, AllocationFlag b) {
    return static_cast<AllocationFlag>(static_cast<u32>(a) & static_cast<u32>(b));
}

struct MemoryAllocation {
    void* mappedData = nullptr;
    u64 offset = 0;
    u64 size = 0;
    void* internal = nullptr;
};

class MemoryAllocator {
public:
    virtual ~MemoryAllocator() = default;
    
    virtual MemoryAllocation allocate(u64 size,
                                      u32 memoryTypeBits,
                                      const AllocationFlag& flags = {}) = 0;
    
    virtual void free(const MemoryAllocation& allocation) = 0;
    
    virtual void* map(const MemoryAllocation& allocation) = 0;
    
    virtual void unmap(const MemoryAllocation& allocation) = 0;
    
    virtual void flush(const MemoryAllocation& allocation,
                       u64 offset = 0, u64 size = ~0ull) = 0;
    
    virtual void invalidate(const MemoryAllocation& allocation,
                            u64 offset = 0, u64 size = ~0ull) = 0;
    
    virtual u32 findMemoryType(u32 typeFilter, MemoryType properties) = 0;
};

}
