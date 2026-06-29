#pragma once

#include <gfx/core/types.h>
#include <vector>

namespace gfx {

enum class AttachmentLoadOp {
    LOAD,
    CLEAR,
    DONT_CARE
};

enum class AttachmentStoreOp {
    STORE,
    DONT_CARE
};

enum class ImageLayout {
    UNDEFINED = 0,
    GENERAL = 1,
    COLOR_ATTACHMENT_OPTIMAL = 2,
    DEPTH_STENCIL_ATTACHMENT_OPTIMAL = 3,
    DEPTH_STENCIL_READ_ONLY_OPTIMAL = 4,
    SHADER_READ_ONLY_OPTIMAL = 5,
    TRANSFER_SRC_OPTIMAL = 6,
    TRANSFER_DST_OPTIMAL = 7,
    PREINITIALIZED = 8,
    DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL = 9,
    DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL = 10,
    PRESENT_SRC_KHR = 11
};

enum class Format {
    UNDEFINED = 0,
    R8G8B8A8_UNORM = 1,
    R8G8B8A8_SRGB = 2,
    R32G32B32A32_SFLOAT = 3,
    D32_SFLOAT = 4,
    D24_UNORM_S8_UINT = 5,
    R32_SFLOAT = 6,
    R32G32_SFLOAT = 7,
    R16G16B16A16_SFLOAT = 8,
    R32G32B32_SFLOAT = 9
};

struct AttachmentDescription {
    Format format;
    AttachmentLoadOp loadOp;
    AttachmentStoreOp storeOp;
    AttachmentLoadOp stencilLoadOp;
    AttachmentStoreOp stencilStoreOp;
    ImageLayout initialLayout;
    ImageLayout finalLayout;
};

struct AttachmentReference {
    u32 attachment;
    ImageLayout layout;
};

struct SubpassDescription {
    std::vector<AttachmentReference> colorAttachments;
    AttachmentReference depthStencilAttachment;
    std::vector<AttachmentReference> inputAttachments;
    std::vector<AttachmentReference> resolveAttachments;
    std::vector<u32> preserveAttachments;
};

struct SubpassDependency {
    u32 srcSubpass;
    u32 dstSubpass;
    PipelineStage srcStageMask;
    PipelineStage dstStageMask;
    MemoryBarrierFlag srcAccessMask;
    MemoryBarrierFlag dstAccessMask;
};

struct RenderPassDesc {
    std::vector<AttachmentDescription> attachments;
    std::vector<SubpassDescription> subpasses;
    std::vector<SubpassDependency> dependencies;
};

class RenderPass {
public:
    virtual ~RenderPass() = default;
    
    virtual const RenderPassDesc& getDesc() const = 0;
    
    virtual void* getNativeHandle() const = 0;
};

}
