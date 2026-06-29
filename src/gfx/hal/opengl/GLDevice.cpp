#include <gfx/hal/opengl/GLDevice.h>
#include <gfx/hal/opengl/GLCommandBuffer.h>
#include <gfx/hal/opengl/GLRenderPass.h>
#include <gfx/hal/opengl/GLFramebuffer.h>
#include <gfx/hal/opengl/GLPipeline.h>
#include <gfx/hal/opengl/GLBuffer.h>
#include <gfx/hal/opengl/GLTexture.h>
#include <gfx/hal/opengl/GLSwapchain.h>
#include <gfx/hal/opengl/GLSemaphore.h>
#include <gfx/hal/opengl/GLFence.h>
#include <gfx/hal/opengl/GLMemoryAllocator.h>
#include <gfx/core/logging.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace gfx {

GLDevice::GLDevice() {
    const unsigned char* vendor = glGetString(GL_VENDOR);
    const unsigned char* renderer = glGetString(GL_RENDERER);
    const unsigned char* version = glGetString(GL_VERSION);
    
    m_deviceName = std::string(reinterpret_cast<const char*>(renderer)) + " (" + 
                   std::string(reinterpret_cast<const char*>(vendor)) + ")";
    
    GFX_LOG_INFO("OpenGL Device created: %s", m_deviceName.c_str());
    GFX_LOG_INFO("OpenGL Version: %s", reinterpret_cast<const char*>(version));
}

GLDevice::~GLDevice() {
    GFX_LOG_INFO("OpenGL Device destroyed");
}

std::unique_ptr<CommandBuffer> GLDevice::createCommandBuffer() {
    return std::make_unique<GLCommandBuffer>();
}

std::unique_ptr<RenderPass> GLDevice::createRenderPass(const RenderPassDesc& desc) {
    return std::make_unique<GLRenderPass>(desc);
}

std::unique_ptr<Framebuffer> GLDevice::createFramebuffer(const FramebufferDesc& desc) {
    return std::make_unique<GLFramebuffer>(desc);
}

std::unique_ptr<Pipeline> GLDevice::createPipeline(const PipelineDesc& desc) {
    return std::make_unique<GLPipeline>(desc);
}

std::unique_ptr<Buffer> GLDevice::createBuffer(const BufferDesc& desc) {
    return std::make_unique<GLBuffer>(desc);
}

std::unique_ptr<Texture> GLDevice::createTexture(const TextureDesc& desc) {
    return std::make_unique<GLTexture>(desc);
}

std::unique_ptr<Swapchain> GLDevice::createSwapchain(const SwapchainDesc& desc) {
    return std::make_unique<GLSwapchain>(desc);
}

std::unique_ptr<Semaphore> GLDevice::createSemaphore() {
    return std::make_unique<GLSemaphore>();
}

std::unique_ptr<Fence> GLDevice::createFence(bool signaled) {
    return std::make_unique<GLFence>(signaled);
}

MemoryAllocator& GLDevice::getMemoryAllocator() {
    static GLMemoryAllocator allocator;
    return allocator;
}

void GLDevice::submit(CommandBuffer* cmd, const std::vector<Semaphore*>& waitSemaphores,
                      const std::vector<Semaphore*>& signalSemaphores, Fence* fence) {
    (void)cmd;
    (void)waitSemaphores;
    (void)signalSemaphores;
    (void)fence;
}

void GLDevice::submit(const std::vector<CommandBuffer*>& cmds, const std::vector<Semaphore*>& waitSemaphores,
                      const std::vector<Semaphore*>& signalSemaphores, Fence* fence) {
    (void)cmds;
    (void)waitSemaphores;
    (void)signalSemaphores;
    (void)fence;
}

void GLDevice::waitIdle() {
    glFinish();
}

void* GLDevice::getNativeHandle() const {
    return nullptr;
}

const std::string& GLDevice::getDeviceName() const {
    return m_deviceName;
}

}
