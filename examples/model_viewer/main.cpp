#include <gfx/core/logging.h>
#include <gfx/core/types.h>
#include <glad/gl.h>
#include <gfx/platform/Window.h>
#include <gfx/hal/Instance.h>
#include <gfx/hal/Device.h>
#include <gfx/hal/Swapchain.h>
#include <gfx/hal/Buffer.h>
#include <gfx/hal/Pipeline.h>
#include <gfx/hal/CommandBuffer.h>
#include <gfx/scene/ModelLoader.h>
#include <gfx/scene/Scene.h>
#include <gfx/scene/Entity.h>
#include <gfx/scene/MeshComponent.h>
#include <gfx/camera/Camera.h>
#include <gfx/camera/OrbitController.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

int main(int argc, char* argv[]) {
    gfx::initLogging();
    GFX_LOG_INFO("MyEngine Model Viewer");
    
    std::string modelPath = "";
    if (argc > 1) {
        modelPath = argv[1];
    }
    
    gfx::Window::Desc windowDesc;
    windowDesc.title = "Model Viewer";
    windowDesc.extent = {1280, 720};
    windowDesc.resizable = true;
    windowDesc.vsync = true;
    
    auto window = gfx::Window::create(windowDesc);
    
    auto instance = gfx::Instance::create("ModelViewer", 1, {});
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
    
    gfx::Camera camera;
    camera.setPosition({0.0f, 5.0f, 10.0f});
    camera.setTarget({0.0f, 0.0f, 0.0f});
    camera.setAspectRatio(1280.0f / 720.0f);
    
    gfx::OrbitController controller(&camera);
    controller.setWindow(window.get());
    controller.setRadius(10.0f);
    
    auto scene = std::make_shared<gfx::Scene>();
    
    if (!modelPath.empty()) {
        auto modelLoader = gfx::ModelLoader::create();
        gfx::ModelLoader::LoadedModel model;
        
        if (modelLoader->loadModel(modelPath, model)) {
            GFX_LOG_INFO("Loaded model: %s with %zu meshes", model.name.c_str(), model.meshes.size());
            
            for (size_t i = 0; i < model.meshes.size(); ++i) {
                auto entity = scene->createEntity("Mesh_" + std::to_string(i));
                auto meshComponent = entity->addComponent<gfx::MeshComponent>();
                meshComponent->setMesh(model.meshes[i]);
            }
        } else {
            GFX_LOG_ERROR("Failed to load model: %s", modelPath.c_str());
        }
    } else {
        GFX_LOG_INFO("No model path provided, creating default cube");
        auto entity = scene->createEntity("Cube");
        auto meshComponent = entity->addComponent<gfx::MeshComponent>();
        meshComponent->setMesh(gfx::Mesh::createCube());
    }
    
    gfx::ShaderModule vertexShader;
    vertexShader.stage = gfx::ShaderStage::VERTEX;
    const char* vertexShaderSource = R"(
        #version 450 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;
        layout (location = 2) in vec2 aTexCoord;
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        
        out vec3 Normal;
        out vec3 FragPos;
        
        void main() {
            FragPos = vec3(model * vec4(aPos, 1.0));
            Normal = mat3(transpose(inverse(model))) * aNormal;
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )";
    vertexShader.code = vertexShaderSource;
    vertexShader.size = strlen(vertexShaderSource);
    
    gfx::ShaderModule fragmentShader;
    fragmentShader.stage = gfx::ShaderStage::FRAGMENT;
    const char* fragmentShaderSource = R"(
        #version 450 core
        out vec4 FragColor;
        
        in vec3 Normal;
        in vec3 FragPos;
        
        uniform vec3 lightPos = vec3(10.0, 10.0, 10.0);
        uniform vec3 viewPos = vec3(0.0, 0.0, 0.0);
        uniform vec3 objectColor = vec3(0.8, 0.5, 0.2);
        
        void main() {
            vec3 ambient = 0.3 * objectColor;
            
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * objectColor;
            
            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = 0.5 * spec * vec3(1.0, 1.0, 1.0);
            
            vec3 result = ambient + diffuse + specular;
            FragColor = vec4(result, 1.0);
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
    
    gfx::VertexInputAttribute attr2;
    attr2.location = 2;
    attr2.binding = 0;
    attr2.format = gfx::Format::R32G32_SFLOAT;
    attr2.offset = sizeof(float) * 6;
    pipelineDesc.vertexAttributes.push_back(attr2);
    
    gfx::VertexInputBinding binding;
    binding.binding = 0;
    binding.stride = sizeof(gfx::Vertex);
    pipelineDesc.vertexBindings.push_back(binding);
    
    pipelineDesc.topology = gfx::PrimitiveTopology::TRIANGLE_LIST;
    
    auto pipeline = device->createPipeline(pipelineDesc);
    
    auto cmdBuffer = device->createCommandBuffer();
    
    double lastTime = glfwGetTime();
    
    while (!window->shouldClose()) {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;
        
        window->pollEvents();
        
        controller.update(deltaTime);
        camera.update();
        
        cmdBuffer->begin();
        
        gfx::Rect2D viewport;
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<gfx::f32>(window->getExtent().width);
        viewport.height = static_cast<gfx::f32>(window->getExtent().height);
        cmdBuffer->setViewport(viewport);
        
        gfx::Rect2D scissor;
        scissor.x = 0.0f;
        scissor.y = 0.0f;
        scissor.width = static_cast<gfx::f32>(window->getExtent().width);
        scissor.height = static_cast<gfx::f32>(window->getExtent().height);
        cmdBuffer->setScissor(scissor);
        
        cmdBuffer->bindPipeline(pipeline.get());
        
        auto entities = scene->getEntitiesWithComponent<gfx::MeshComponent>();
        for (auto entity : entities) {
            auto meshComponent = entity->getComponent<gfx::MeshComponent>();
            if (meshComponent) {
                auto mesh = meshComponent->getMesh();
                if (mesh) {
                    cmdBuffer->bindVertexBuffer(0, nullptr, 0);
                    cmdBuffer->drawIndexed(static_cast<gfx::u32>(mesh->getIndices().size()), 1, 0, 0, 0);
                }
            }
        }
        
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