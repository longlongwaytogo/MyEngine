add_library(gfx STATIC)

target_include_directories(gfx
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include
        ${THIRD_PARTY_DIR}/glfw/deps
        ${THIRD_PARTY_DIR}/slang-2026.12-windows-x86_64/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(gfx
    PRIVATE
        ${THIRD_PARTY_DIR}/glad/glad_loader.cpp
        core/logging.cpp
        scene/Transform.cpp
        scene/Entity.cpp
        scene/Scene.cpp
        scene/Mesh.cpp
        scene/MeshComponent.cpp
        scene/ModelLoader.cpp
        platform/glfw_window.cpp
        shader/ShaderCompiler.cpp
        shader/Shader.cpp
        rendergraph/RenderGraph.cpp
        rendergraph/RenderPass.cpp
        rendergraph/Resource.cpp
        camera/Camera.cpp
        camera/OrbitController.cpp
        hal/opengl/GLInstance.cpp
        hal/opengl/GLDevice.cpp
        hal/opengl/GLCommandBuffer.cpp
        hal/opengl/GLRenderPass.cpp
        hal/opengl/GLFramebuffer.cpp
        hal/opengl/GLPipeline.cpp
        hal/opengl/GLBuffer.cpp
        hal/opengl/GLTexture.cpp
        hal/opengl/GLSwapchain.cpp
        hal/opengl/GLMemoryAllocator.cpp
        hal/opengl/GLSemaphore.cpp
        hal/opengl/GLFence.cpp
)

target_link_libraries(gfx
    PUBLIC
        glfw
        glm::glm
)

if(GFX_ENABLE_LOGGING)
    target_link_libraries(gfx PUBLIC spdlog::spdlog)
endif()

target_link_libraries(gfx PUBLIC slang::slang)
target_link_libraries(gfx PUBLIC assimp)
