#include <gfx/core/logging.h>
#include <gfx/platform/Window.h>
#include <gfx/hal/Instance.h>
#include <gfx/hal/Device.h>
#include <gfx/hal/Swapchain.h>
#include <gfx/hal/Buffer.h>
#include <gfx/hal/Pipeline.h>
#include <gfx/hal/CommandBuffer.h>
#include <gfx/core/types.h>
#include <glad/gl.h>

int main() {
    gfx::initLogging();
    GFX_LOG_INFO("MyEngine Triangle Example");
    
    gfx::Window::Desc windowDesc;
    windowDesc.title = "Triangle Example";
    windowDesc.extent = {800, 600};
    windowDesc.resizable = true;
    windowDesc.vsync = true;
    
    auto window = gfx::Window::create(windowDesc);
    
    auto instance = gfx::Instance::create("TriangleExample", 1, {});
    auto devices = instance->enumerateDevices();
    if (devices.empty()) {
        GFX_LOG_ERROR("No devices available");
        return 1;
    }
    
    auto device = std::move(devices[0]);
    GFX_LOG_INFO("Using device: %s", device->getDeviceName().c_str());
    
    gfx::SwapchainDesc swapchainDesc;
    swapchainDesc.window = window.get();
    swapchainDesc.extent = window->getExtent();
    swapchainDesc.format = gfx::Format::R8G8B8A8_UNORM;
    
    auto swapchain = device->createSwapchain(swapchainDesc);
    
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };
    
    gfx::BufferDesc vertexBufferDesc;
    vertexBufferDesc.size = sizeof(vertices);
    vertexBufferDesc.usage = gfx::BufferUsage::VERTEX;
    vertexBufferDesc.memoryProperty = gfx::MemoryProperty::DEVICE_LOCAL;
    vertexBufferDesc.initialData = vertices;
    
    auto vertexBuffer = device->createBuffer(vertexBufferDesc);
    
    gfx::ShaderModule vertexShader;
    vertexShader.stage = gfx::ShaderStage::VERTEX;
    const char* vertexShaderSource = R"(
        #version 450 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        out vec3 ourColor;
        void main() {
            gl_Position = vec4(aPos, 1.0);
            ourColor = aColor;
        }
    )";
    vertexShader.code = vertexShaderSource;
    vertexShader.size = strlen(vertexShaderSource);
    
    gfx::ShaderModule fragmentShader;
    fragmentShader.stage = gfx::ShaderStage::FRAGMENT;
    const char* fragmentShaderSource = R"(
        #version 450 core
        in vec3 ourColor;
        out vec4 FragColor;
        void main() {
            FragColor = vec4(ourColor, 1.0);
        }
    )";
    fragmentShader.code = fragmentShaderSource;
    fragmentShader.size = strlen(fragmentShaderSource);
    
    gfx::PipelineDesc pipelineDesc;
    pipelineDesc.shaders.push_back(vertexShader);
    pipelineDesc.shaders.push_back(fragmentShader);
    
    gfx::VertexInputAttribute attr0;
    attr0.location = 0;
    attr0.binding = 0;
    attr0.format = gfx::Format::R32G32B32_SFLOAT;
    attr0.offset = 0;
    pipelineDesc.vertexAttributes.push_back(attr0);
    
    gfx::VertexInputAttribute attr1;
    attr1.location = 1;
    attr1.binding = 0;
    attr1.format = gfx::Format::R32G32B32_SFLOAT;
    attr1.offset = sizeof(float) * 3;
    pipelineDesc.vertexAttributes.push_back(attr1);
    
    gfx::VertexInputBinding binding;
    binding.binding = 0;
    binding.stride = sizeof(float) * 6;
    pipelineDesc.vertexBindings.push_back(binding);
    
    pipelineDesc.topology = gfx::PrimitiveTopology::TRIANGLE_LIST;
    
    auto pipeline = device->createPipeline(pipelineDesc);
    
    auto cmdBuffer = device->createCommandBuffer();
    
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    
    while (!window->shouldClose()) {
        window->pollEvents();
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        cmdBuffer->begin();
        
        gfx::Rect2D viewport;
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<gfx::f32>(window->getExtent().width);
        viewport.height = static_cast<gfx::f32>(window->getExtent().height);
        cmdBuffer->setViewport(viewport);
        
        cmdBuffer->bindPipeline(pipeline.get());
        cmdBuffer->bindVertexBuffer(0, vertexBuffer.get(), 0);
        cmdBuffer->draw(3, 1, 0, 0);
        cmdBuffer->end();
        
        std::vector<gfx::Semaphore*> emptySemaphores;
        device->submit(cmdBuffer.get(), emptySemaphores, emptySemaphores, nullptr);
        swapchain->present(nullptr);
        window->swapBuffers();
    }
    
    device->waitIdle();
    
    gfx::shutdownLogging();
    return 0;
}