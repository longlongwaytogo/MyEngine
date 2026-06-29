#include <gfx/hal/opengl/GLCommandBuffer.h>
#include <gfx/hal/Pipeline.h>
#include <gfx/core/logging.h>
#include <glad/gl.h>

namespace gfx {

GLCommandBuffer::GLCommandBuffer() {
    glGenVertexArrays(1, &m_vao);
    GFX_LOG_INFO("GLCommandBuffer created with VAO: %u", m_vao);
}

GLCommandBuffer::~GLCommandBuffer() {
    if (m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
        GFX_LOG_INFO("GLCommandBuffer destroyed");
    }
}

void GLCommandBuffer::begin() {
    m_recording = true;
    glBindVertexArray(m_vao);
}

void GLCommandBuffer::end() {
    m_recording = false;
    glBindVertexArray(0);
}

void GLCommandBuffer::beginRenderPass(RenderPass* renderPass, Framebuffer* framebuffer,
                                      const Rect2D& renderArea, const std::vector<ClearValue>& clearValues) {
    (void)renderPass;
    (void)framebuffer;
    (void)renderArea;
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    if (!clearValues.empty()) {
        auto& clear = clearValues[0];
        glClearColor(clear.color.r, clear.color.g, clear.color.b, clear.color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void GLCommandBuffer::endRenderPass() {
}

void GLCommandBuffer::bindPipeline(Pipeline* pipeline) {
    if (!pipeline) {
        glUseProgram(0);
        m_currentProgram = 0;
        return;
    }
    
    unsigned int program = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(pipeline->getNativeHandle()));
    glUseProgram(program);
    m_currentProgram = program;
}

void GLCommandBuffer::bindVertexBuffer(u32 binding, Buffer* buffer, u64 offset) {
    if (!buffer) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return;
    }
    
    unsigned int vbo = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(buffer->getNativeHandle()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    switch (binding) {
        case 0:
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 
                                 reinterpret_cast<void*>(offset));
            break;
        case 1:
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 
                                 reinterpret_cast<void*>(offset));
            break;
        case 2:
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 
                                 reinterpret_cast<void*>(offset));
            break;
        default:
            break;
    }
}

void GLCommandBuffer::bindIndexBuffer(Buffer* buffer, u64 offset, IndexType indexType) {
    (void)offset;
    (void)indexType;
    
    if (!buffer) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        return;
    }
    
    unsigned int ebo = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(buffer->getNativeHandle()));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void GLCommandBuffer::setViewport(const Rect2D& viewport) {
    glViewport(static_cast<GLint>(viewport.x), static_cast<GLint>(viewport.y),
               static_cast<GLsizei>(viewport.width), static_cast<GLsizei>(viewport.height));
}

void GLCommandBuffer::setScissor(const Rect2D& scissor) {
    glScissor(static_cast<GLint>(scissor.x), static_cast<GLint>(scissor.y),
              static_cast<GLsizei>(scissor.width), static_cast<GLsizei>(scissor.height));
}

void GLCommandBuffer::draw(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance) {
    (void)instanceCount;
    (void)firstInstance;
    glDrawArrays(GL_TRIANGLES, static_cast<GLint>(firstVertex), static_cast<GLsizei>(vertexCount));
}

void GLCommandBuffer::drawIndexed(u32 indexCount, u32 instanceCount, u32 firstIndex, i32 vertexOffset, u32 firstInstance) {
    (void)instanceCount;
    (void)firstInstance;
    (void)vertexOffset;
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT,
                   reinterpret_cast<const void*>(firstIndex * sizeof(u32)));
}

void GLCommandBuffer::drawIndirect(Buffer* indirectBuffer, u64 offset, u32 drawCount, u32 stride) {
    (void)indirectBuffer;
    (void)offset;
    (void)drawCount;
    (void)stride;
}

void GLCommandBuffer::drawIndexedIndirect(Buffer* indirectBuffer, u64 offset, u32 drawCount, u32 stride) {
    (void)indirectBuffer;
    (void)offset;
    (void)drawCount;
    (void)stride;
}

void GLCommandBuffer::copyBufferToBuffer(Buffer* src, Buffer* dst, const std::vector<BufferCopyRegion>& regions) {
    (void)src;
    (void)dst;
    (void)regions;
}

void GLCommandBuffer::copyBufferToTexture(Buffer* src, Texture* dst, const TextureCopyRegion& region) {
    (void)src;
    (void)dst;
    (void)region;
}

void GLCommandBuffer::copyTextureToBuffer(Texture* src, Buffer* dst, const TextureCopyRegion& region) {
    (void)src;
    (void)dst;
    (void)region;
}

void GLCommandBuffer::copyTextureToTexture(Texture* src, Texture* dst, const TextureCopyRegion& region) {
    (void)src;
    (void)dst;
    (void)region;
}

void GLCommandBuffer::pipelineBarrier(PipelineStage srcStage, PipelineStage dstStage,
                                      const std::vector<MemoryBarrier>& memoryBarriers,
                                      const std::vector<BufferMemoryBarrier>& bufferBarriers,
                                      const std::vector<TextureMemoryBarrier>& textureBarriers) {
    (void)srcStage;
    (void)dstStage;
    (void)memoryBarriers;
    (void)bufferBarriers;
    (void)textureBarriers;
}

void GLCommandBuffer::pushConstants(u32 offset, u32 size, const void* data) {
    (void)offset;
    (void)size;
    (void)data;
}

void* GLCommandBuffer::getNativeHandle() const {
    return nullptr;
}

}
