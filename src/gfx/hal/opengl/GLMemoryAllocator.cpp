#include <gfx/hal/opengl/GLMemoryAllocator.h>
#include <cstdlib>

namespace gfx {

GLMemoryAllocator::GLMemoryAllocator() {
}

GLMemoryAllocator::~GLMemoryAllocator() {
}

MemoryAllocation GLMemoryAllocator::allocate(u64 size, u32 memoryTypeBits, const AllocationFlag& flags) {
    (void)memoryTypeBits;
    (void)flags;
    MemoryAllocation allocation;
    allocation.mappedData = malloc(static_cast<size_t>(size));
    allocation.size = size;
    allocation.offset = 0;
    allocation.internal = allocation.mappedData;
    return allocation;
}

void GLMemoryAllocator::free(const MemoryAllocation& allocation) {
    if (allocation.internal) {
        ::free(allocation.internal);
    }
}

void* GLMemoryAllocator::map(const MemoryAllocation& allocation) {
    return allocation.mappedData;
}

void GLMemoryAllocator::unmap(const MemoryAllocation& allocation) {
    (void)allocation;
}

void GLMemoryAllocator::flush(const MemoryAllocation& allocation, u64 offset, u64 size) {
    (void)allocation;
    (void)offset;
    (void)size;
}

void GLMemoryAllocator::invalidate(const MemoryAllocation& allocation, u64 offset, u64 size) {
    (void)allocation;
    (void)offset;
    (void)size;
}

u32 GLMemoryAllocator::findMemoryType(u32 typeFilter, MemoryType properties) {
    (void)typeFilter;
    (void)properties;
    return 0;
}

}