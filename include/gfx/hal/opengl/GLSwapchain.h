#pragma once

#include <gfx/hal/Swapchain.h>

namespace gfx {

class GLSwapchain : public Swapchain {
public:
    GLSwapchain(const SwapchainDesc& desc);
    ~GLSwapchain() override;
    
    const SwapchainDesc& getDesc() const override;
    u32 acquireNextImage(Semaphore* signalSemaphore = nullptr, Fence* signalFence = nullptr) override;
    Texture* getImage(u32 index) override;
    u32 getImageCount() const override;
    void present(Semaphore* waitSemaphore = nullptr) override;
    void resize(const Extent2D& extent) override;
    void* getNativeHandle() const override;
    
private:
    SwapchainDesc m_desc;
};

}