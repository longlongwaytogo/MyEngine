#pragma once

#include <gfx/hal/RenderPass.h>

namespace gfx {

class GLRenderPass : public RenderPass {
public:
    GLRenderPass(const RenderPassDesc& desc);
    ~GLRenderPass() override;
    
    const RenderPassDesc& getDesc() const override;
    void* getNativeHandle() const override;
    
private:
    RenderPassDesc m_desc;
};

}
