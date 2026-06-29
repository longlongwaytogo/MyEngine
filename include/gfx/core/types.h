#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <cstdint>
#include <cstddef>
#include <memory>

namespace gfx {

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using f32 = float;
using f64 = double;

using usize = size_t;

struct Rect2D {
    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 width = 0.0f;
    f32 height = 0.0f;
};

struct Extent2D {
    u32 width = 0;
    u32 height = 0;
};

struct Extent3D {
    u32 width = 0;
    u32 height = 0;
    u32 depth = 0;
};

struct ClearValue {
    union {
        struct {
            f32 r, g, b, a;
        } color;
        struct {
            f32 depth;
            u32 stencil;
        } depthStencil;
    };
    
    ClearValue() : color{0.0f, 0.0f, 0.0f, 1.0f} {}
    
    static ClearValue Color(f32 r, f32 g, f32 b, f32 a = 1.0f) {
        ClearValue v;
        v.color.r = r;
        v.color.g = g;
        v.color.b = b;
        v.color.a = a;
        return v;
    }
    
    static ClearValue DepthStencil(f32 depth, u32 stencil = 0) {
        ClearValue v;
        v.depthStencil.depth = depth;
        v.depthStencil.stencil = stencil;
        return v;
    }
};

enum class IndexType {
    UINT16,
    UINT32
};

enum class PipelineStage {
    TOP_OF_PIPE = 0,
    DRAW_INDIRECT = 1,
    VERTEX_INPUT = 2,
    VERTEX_SHADER = 3,
    TESSELLATION_CONTROL_SHADER = 4,
    TESSELLATION_EVALUATION_SHADER = 5,
    GEOMETRY_SHADER = 6,
    FRAGMENT_SHADER = 7,
    EARLY_FRAGMENT_TESTS = 8,
    LATE_FRAGMENT_TESTS = 9,
    COLOR_ATTACHMENT_OUTPUT = 10,
    COMPUTE_SHADER = 11,
    TRANSFER = 12,
    BOTTOM_OF_PIPE = 13,
    HOST = 14,
    ALL_GRAPHICS = 15,
    ALL_COMMANDS = 16
};

enum class MemoryBarrierFlag {
    NONE = 0,
    TRANSFER_READ = 1,
    TRANSFER_WRITE = 2,
    UNIFORM_TEXEL_BUFFER_READ = 4,
    STORAGE_TEXEL_BUFFER_READ = 8,
    UNIFORM_BUFFER_READ = 16,
    STORAGE_BUFFER_READ = 32,
    STORAGE_BUFFER_WRITE = 64,
    INDEX_BUFFER_READ = 128,
    VERTEX_ATTRIBUTE_READ = 256,
    INDIRECT_BUFFER_READ = 512,
    COLOR_ATTACHMENT_READ = 1024,
    COLOR_ATTACHMENT_WRITE = 2048,
    DEPTH_STENCIL_ATTACHMENT_READ = 4096,
    DEPTH_STENCIL_ATTACHMENT_WRITE = 8192,
    SHADER_READ = 16384,
    SHADER_WRITE = 32768,
    ACCELERATION_STRUCTURE_READ = 65536,
    ACCELERATION_STRUCTURE_WRITE = 131072,
    FRAGMENT_SHADING_RATE_ATTACHMENT_READ = 262144,
    COMMAND_PREPROCESS_READ = 524288,
    COMMAND_PREPROCESS_WRITE = 1048576,
    TRANSFORM_FEEDBACK_WRITE = 2097152,
    TRANSFORM_FEEDBACK_COUNTER_READ = 4194304,
    CONCURRENT_ACCESS = 8388608
};

struct MemoryBarrier {
    PipelineStage srcStage;
    PipelineStage dstStage;
    MemoryBarrierFlag flags;
};

struct BufferMemoryBarrier {
    PipelineStage srcStage;
    PipelineStage dstStage;
    MemoryBarrierFlag srcAccess;
    MemoryBarrierFlag dstAccess;
    void* buffer;
    u64 offset;
    u64 size;
};

struct TextureMemoryBarrier {
    PipelineStage srcStage;
    PipelineStage dstStage;
    MemoryBarrierFlag srcAccess;
    MemoryBarrierFlag dstAccess;
    void* texture;
    u32 mipLevel;
    u32 arrayLayer;
};

struct BufferCopyRegion {
    u64 srcOffset;
    u64 dstOffset;
    u64 size;
};

struct TextureCopyRegion {
    void* srcBuffer;
    u64 bufferOffset;
    u32 bufferRowLength;
    u32 bufferImageHeight;
    void* dstTexture;
    u32 mipLevel;
    u32 baseArrayLayer;
    u32 layerCount;
    Extent3D extent;
};

}
