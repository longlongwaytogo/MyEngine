#include <gfx/hal/opengl/GLSwapchain.h>

namespace gfx {

GLSwapchain::GLSwapchain(const SwapchainDesc& desc) : m_desc(desc) {
}

GLSwapchain::~GLSwapchain() {
}

const SwapchainDesc& GLSwapchain::getDesc() const {
    return m_desc;
}

u32 GLSwapchain::acquireNextImage(Semaphore* signalSemaphore, Fence* signalFence) {
    (void)signalSemaphore;
    (void)signalFence;
    return 0;
}

Texture* GLSwapchain::getImage(u32 index) {
    (void)index;
    return nullptr;
}

u32 GLSwapchain::getImageCount() const {
    return 1;
}

void GLSwapchain::present(Semaphore* waitSemaphore) {
    (void)waitSemaphore;
}

void GLSwapchain::resize(const Extent2D& extent) {
    m_desc.extent = extent;
}

void* GLSwapchain::getNativeHandle() const {
    return nullptr;
}

}