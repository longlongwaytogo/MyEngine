#pragma once

#include <gfx/core/types.h>
#include <gfx/hal/RenderPass.h>
#include <gfx/hal/Framebuffer.h>
#include <gfx/hal/Pipeline.h>
#include <gfx/hal/Buffer.h>
#include <gfx/hal/Texture.h>
#include <vector>

namespace gfx {

class CommandBuffer {
public:
    virtual ~CommandBuffer() = default;
    
    virtual void begin() = 0;
    
    virtual void end() = 0;
    
    virtual void beginRenderPass(RenderPass* renderPass, Framebuffer* framebuffer,
                                 const Rect2D& renderArea,
                                 const std::vector<ClearValue>& clearValues) = 0;
    
    virtual void endRenderPass() = 0;
    
    virtual void bindPipeline(Pipeline* pipeline) = 0;
    
    virtual void bindVertexBuffer(u32 binding, Buffer* buffer, u64 offset = 0) = 0;
    
    virtual void bindIndexBuffer(Buffer* buffer, u64 offset = 0, IndexType indexType = IndexType::UINT32) = 0;
    
    virtual void setViewport(const Rect2D& viewport) = 0;
    
    virtual void setScissor(const Rect2D& scissor) = 0;
    
    virtual void draw(u32 vertexCount, u32 instanceCount = 1,
                      u32 firstVertex = 0, u32 firstInstance = 0) = 0;
    
    virtual void drawIndexed(u32 indexCount, u32 instanceCount = 1,
                             u32 firstIndex = 0, i32 vertexOffset = 0,
                             u32 firstInstance = 0) = 0;
    
    virtual void drawIndirect(Buffer* indirectBuffer, u64 offset = 0,
                             u32 drawCount = 1, u32 stride = 0) = 0;
    
    virtual void drawIndexedIndirect(Buffer* indirectBuffer, u64 offset = 0,
                                     u32 drawCount = 1, u32 stride = 0) = 0;
    
    virtual void copyBufferToBuffer(Buffer* src, Buffer* dst,
                                    const std::vector<BufferCopyRegion>& regions) = 0;
    
    virtual void copyBufferToTexture(Buffer* src, Texture* dst,
                                     const TextureCopyRegion& region) = 0;
    
    virtual void copyTextureToBuffer(Texture* src, Buffer* dst,
                                     const TextureCopyRegion& region) = 0;
    
    virtual void copyTextureToTexture(Texture* src, Texture* dst,
                                       const TextureCopyRegion& region) = 0;
    
    virtual void pipelineBarrier(PipelineStage srcStage, PipelineStage dstStage,
                                 const std::vector<MemoryBarrier>& memoryBarriers = {},
                                 const std::vector<BufferMemoryBarrier>& bufferBarriers = {},
                                 const std::vector<TextureMemoryBarrier>& textureBarriers = {}) = 0;
    
    virtual void pushConstants(u32 offset, u32 size, const void* data) = 0;
    
    virtual void* getNativeHandle() const = 0;
};

}
