#pragma once

#include <gfx/core/types.h>
#include <gfx/hal/Semaphore.h>
#include <gfx/hal/Fence.h>
#include <gfx/hal/CommandBuffer.h>
#include <gfx/hal/RenderPass.h>
#include <gfx/hal/Framebuffer.h>
#include <gfx/hal/Pipeline.h>
#include <gfx/hal/Buffer.h>
#include <gfx/hal/Texture.h>
#include <gfx/hal/Swapchain.h>
#include <gfx/hal/MemoryAllocator.h>
#include <vector>
#include <memory>
#include <string>

namespace gfx {

class Device {
public:
    virtual ~Device() = default;
    
    virtual std::unique_ptr<CommandBuffer> createCommandBuffer() = 0;
    
    virtual std::unique_ptr<RenderPass> createRenderPass(const RenderPassDesc& desc) = 0;
    
    virtual std::unique_ptr<Framebuffer> createFramebuffer(const FramebufferDesc& desc) = 0;
    
    virtual std::unique_ptr<Pipeline> createPipeline(const PipelineDesc& desc) = 0;
    
    virtual std::unique_ptr<Buffer> createBuffer(const BufferDesc& desc) = 0;
    
    virtual std::unique_ptr<Texture> createTexture(const TextureDesc& desc) = 0;
    
    virtual std::unique_ptr<Swapchain> createSwapchain(const SwapchainDesc& desc) = 0;
    
    virtual std::unique_ptr<Semaphore> createSemaphore() = 0;
    
    virtual std::unique_ptr<Fence> createFence(bool signaled = false) = 0;
    
    virtual MemoryAllocator& getMemoryAllocator() = 0;
    
    virtual void submit(CommandBuffer* cmd,
                        const std::vector<Semaphore*>& waitSemaphores = {},
                        const std::vector<Semaphore*>& signalSemaphores = {},
                        Fence* fence = nullptr) = 0;
    
    virtual void submit(const std::vector<CommandBuffer*>& cmds,
                        const std::vector<Semaphore*>& waitSemaphores = {},
                        const std::vector<Semaphore*>& signalSemaphores = {},
                        Fence* fence = nullptr) = 0;
    
    virtual void waitIdle() = 0;
    
    virtual void* getNativeHandle() const = 0;
    
    virtual const std::string& getDeviceName() const = 0;
};

}
