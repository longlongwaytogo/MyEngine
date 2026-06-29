#pragma once

#include <gfx/hal/Framebuffer.h>

namespace gfx {

class GLFramebuffer : public Framebuffer {
public:
    GLFramebuffer(const FramebufferDesc& desc);
    ~GLFramebuffer() override;
    
    const FramebufferDesc& getDesc() const override;
    void* getNativeHandle() const override;
    
private:
    FramebufferDesc m_desc;
    unsigned int m_fbo = 0;
};

}
