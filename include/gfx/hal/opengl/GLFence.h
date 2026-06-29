#pragma once

#include <gfx/hal/Fence.h>

namespace gfx {

class GLFence : public Fence {
public:
    GLFence(bool signaled);
    ~GLFence() override;
    
    void wait(u64 timeout = ~0ull) override;
    bool isSignaled() const override;
    void reset() override;
    void* getNativeHandle() const override;
    
private:
    bool m_signaled;
};

}