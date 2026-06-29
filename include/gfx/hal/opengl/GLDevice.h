#pragma once

#include <gfx/hal/Device.h>

namespace gfx {

class GLDevice : public Device {
public:
    GLDevice();
    ~GLDevice() override;
    
    std::unique_ptr<CommandBuffer> createCommandBuffer() override;
    std::unique_ptr<RenderPass> createRenderPass(const RenderPassDesc& desc) override;
    std::unique_ptr<Framebuffer> createFramebuffer(const FramebufferDesc& desc) override;
    std::unique_ptr<Pipeline> createPipeline(const PipelineDesc& desc) override;
    std::unique_ptr<Buffer> createBuffer(const BufferDesc& desc) override;
    std::unique_ptr<Texture> createTexture(const TextureDesc& desc) override;
    std::unique_ptr<Swapchain> createSwapchain(const SwapchainDesc& desc) override;
    std::unique_ptr<Semaphore> createSemaphore() override;
    std::unique_ptr<Fence> createFence(bool signaled) override;
    
    MemoryAllocator& getMemoryAllocator() override;
    
    void submit(CommandBuffer* cmd, const std::vector<Semaphore*>& waitSemaphores,
                const std::vector<Semaphore*>& signalSemaphores, Fence* fence) override;
    
    void submit(const std::vector<CommandBuffer*>& cmds, const std::vector<Semaphore*>& waitSemaphores,
                const std::vector<Semaphore*>& signalSemaphores, Fence* fence) override;
    
    void waitIdle() override;
    void* getNativeHandle() const override;
    const std::string& getDeviceName() const override;
    
private:
    std::string m_deviceName;
};

}
