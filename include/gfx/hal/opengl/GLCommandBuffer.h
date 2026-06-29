#pragma once

#include <gfx/hal/CommandBuffer.h>

namespace gfx {

class GLCommandBuffer : public CommandBuffer {
public:
    GLCommandBuffer();
    ~GLCommandBuffer() override;
    
    void begin() override;
    void end() override;
    
    void beginRenderPass(RenderPass* renderPass, Framebuffer* framebuffer,
                         const Rect2D& renderArea, const std::vector<ClearValue>& clearValues) override;
    void endRenderPass() override;
    
    void bindPipeline(Pipeline* pipeline) override;
    void bindVertexBuffer(u32 binding, Buffer* buffer, u64 offset) override;
    void bindIndexBuffer(Buffer* buffer, u64 offset, IndexType indexType) override;
    void setViewport(const Rect2D& viewport) override;
    void setScissor(const Rect2D& scissor) override;
    
    void draw(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance) override;
    void drawIndexed(u32 indexCount, u32 instanceCount, u32 firstIndex, i32 vertexOffset, u32 firstInstance) override;
    void drawIndirect(Buffer* indirectBuffer, u64 offset, u32 drawCount, u32 stride) override;
    void drawIndexedIndirect(Buffer* indirectBuffer, u64 offset, u32 drawCount, u32 stride) override;
    
    void copyBufferToBuffer(Buffer* src, Buffer* dst, const std::vector<BufferCopyRegion>& regions) override;
    void copyBufferToTexture(Buffer* src, Texture* dst, const TextureCopyRegion& region) override;
    void copyTextureToBuffer(Texture* src, Buffer* dst, const TextureCopyRegion& region) override;
    void copyTextureToTexture(Texture* src, Texture* dst, const TextureCopyRegion& region) override;
    
    void pipelineBarrier(PipelineStage srcStage, PipelineStage dstStage,
                         const std::vector<MemoryBarrier>& memoryBarriers,
                         const std::vector<BufferMemoryBarrier>& bufferBarriers,
                         const std::vector<TextureMemoryBarrier>& textureBarriers) override;
    
    void pushConstants(u32 offset, u32 size, const void* data) override;
    
    void* getNativeHandle() const override;
    
private:
    bool m_recording = false;
    unsigned int m_vao = 0;
    unsigned int m_currentProgram = 0;
};

}
