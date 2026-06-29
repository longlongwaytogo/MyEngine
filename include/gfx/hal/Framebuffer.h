#pragma once

#include <gfx/core/types.h>
#include <gfx/hal/RenderPass.h>
#include <vector>

namespace gfx {

class RenderPass;
class Texture;

struct FramebufferDesc {
    RenderPass* renderPass;
    std::vector<Texture*> attachments;
    Extent2D extent;
    u32 layers = 1;
};

class Framebuffer {
public:
    virtual ~Framebuffer() = default;
    
    virtual const FramebufferDesc& getDesc() const = 0;
    
    virtual void* getNativeHandle() const = 0;
};

}
