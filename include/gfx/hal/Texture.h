#pragma once

#include <gfx/core/types.h>
#include <gfx/hal/RenderPass.h>
#include <gfx/hal/Buffer.h>
#include <memory>

namespace gfx {

enum class TextureType {
    TYPE_1D = 0,
    TYPE_2D = 1,
    TYPE_3D = 2,
    TYPE_CUBE = 3,
    TYPE_1D_ARRAY = 4,
    TYPE_2D_ARRAY = 5,
    TYPE_CUBE_ARRAY = 6
};

enum class TextureUsage {
    TRANSFER_SRC = 1,
    TRANSFER_DST = 2,
    SAMPLED = 4,
    STORAGE = 8,
    COLOR_ATTACHMENT = 16,
    DEPTH_STENCIL_ATTACHMENT = 32,
    INPUT_ATTACHMENT = 64
};

enum class SamplerFilter {
    NEAREST = 0,
    LINEAR = 1
};

enum class SamplerAddressMode {
    REPEAT = 0,
    MIRRORED_REPEAT = 1,
    CLAMP_TO_EDGE = 2,
    CLAMP_TO_BORDER = 3,
    MIRROR_CLAMP_TO_EDGE = 4
};

struct TextureDesc {
    TextureType type = TextureType::TYPE_2D;
    Format format = Format::R8G8B8A8_UNORM;
    Extent3D extent;
    u32 mipLevels = 1;
    u32 arrayLayers = 1;
    u32 samples = 1;
    TextureUsage usage = TextureUsage::SAMPLED;
    MemoryProperty memoryProperty = MemoryProperty::DEVICE_LOCAL;
    const void* initialData = nullptr;
    u32 initialDataSize = 0;
};

struct SamplerDesc {
    SamplerFilter magFilter = SamplerFilter::LINEAR;
    SamplerFilter minFilter = SamplerFilter::LINEAR;
    SamplerFilter mipmapMode = SamplerFilter::LINEAR;
    SamplerAddressMode addressModeU = SamplerAddressMode::REPEAT;
    SamplerAddressMode addressModeV = SamplerAddressMode::REPEAT;
    SamplerAddressMode addressModeW = SamplerAddressMode::REPEAT;
    f32 mipLodBias = 0.0f;
    bool anisotropyEnable = false;
    f32 maxAnisotropy = 1.0f;
    bool compareEnable = false;
    f32 minLod = 0.0f;
    f32 maxLod = 1000.0f;
};

class Texture {
public:
    virtual ~Texture() = default;
    
    virtual const TextureDesc& getDesc() const = 0;
    
    virtual void* getNativeHandle() const = 0;
};

class Sampler {
public:
    virtual ~Sampler() = default;
    
    virtual const SamplerDesc& getDesc() const = 0;
    
    virtual void* getNativeHandle() const = 0;
};

}
