#pragma once

#include <gfx/core/types.h>
#include <gfx/hal/Texture.h>
#include <gfx/hal/Semaphore.h>
#include <gfx/hal/Fence.h>
#include <gfx/platform/Window.h>
#include <memory>

namespace gfx {

class Window;

enum class PresentMode {
    IMMEDIATE = 0,
    MAILBOX = 1,
    FIFO = 2,
    FIFO_RELAXED = 3
};

enum class CompositeAlpha {
    OPAQUE = 0,
    PRE_MULTIPLIED = 1,
    POST_MULTIPLIED = 2,
    INHERIT = 3
};

struct SwapchainDesc {
    Window* window = nullptr;
    Extent2D extent;
    Format format = Format::R8G8B8A8_UNORM;
    u32 imageCount = 2;
    PresentMode presentMode = PresentMode::FIFO;
    CompositeAlpha compositeAlpha = CompositeAlpha::OPAQUE;
    bool clipped = true;
};

class Swapchain {
public:
    virtual ~Swapchain() = default;
    
    virtual const SwapchainDesc& getDesc() const = 0;
    
    virtual u32 acquireNextImage(Semaphore* signalSemaphore = nullptr,
                                  Fence* signalFence = nullptr) = 0;
    
    virtual Texture* getImage(u32 index) = 0;
    
    virtual u32 getImageCount() const = 0;
    
    virtual void present(Semaphore* waitSemaphore = nullptr) = 0;
    
    virtual void resize(const Extent2D& extent) = 0;
    
    virtual void* getNativeHandle() const = 0;
};

}
