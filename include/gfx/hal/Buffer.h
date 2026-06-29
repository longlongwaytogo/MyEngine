#pragma once

#include <gfx/core/types.h>
#include <memory>

namespace gfx {

enum class BufferUsage : u32 {
    VERTEX = 1,
    INDEX = 2,
    UNIFORM = 4,
    STORAGE = 8,
    TRANSFER_SRC = 16,
    TRANSFER_DST = 32,
    INDIRECT = 64
};

inline BufferUsage operator|(BufferUsage a, BufferUsage b) {
    return static_cast<BufferUsage>(static_cast<u32>(a) | static_cast<u32>(b));
}

inline BufferUsage operator&(BufferUsage a, BufferUsage b) {
    return static_cast<BufferUsage>(static_cast<u32>(a) & static_cast<u32>(b));
}

enum class MemoryProperty : u32 {
    DEVICE_LOCAL = 1,
    HOST_VISIBLE = 2,
    HOST_COHERENT = 4,
    HOST_CACHED = 8,
    LAZILY_ALLOCATED = 16
};

inline MemoryProperty operator|(MemoryProperty a, MemoryProperty b) {
    return static_cast<MemoryProperty>(static_cast<u32>(a) | static_cast<u32>(b));
}

inline MemoryProperty operator&(MemoryProperty a, MemoryProperty b) {
    return static_cast<MemoryProperty>(static_cast<u32>(a) & static_cast<u32>(b));
}

struct BufferDesc {
    u64 size = 0;
    BufferUsage usage = BufferUsage::VERTEX;
    MemoryProperty memoryProperty = MemoryProperty::DEVICE_LOCAL;
    const void* initialData = nullptr;
};

class Buffer {
public:
    virtual ~Buffer() = default;
    
    virtual const BufferDesc& getDesc() const = 0;
    
    virtual void* map(u64 offset = 0, u64 size = ~0ull) = 0;
    
    virtual void unmap() = 0;
    
    virtual void update(u64 offset, u64 size, const void* data) = 0;
    
    virtual void* getNativeHandle() const = 0;
};

}
