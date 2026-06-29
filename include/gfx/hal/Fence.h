#pragma once

#include <gfx/core/types.h>

namespace gfx {

class Fence {
public:
    virtual ~Fence() = default;
    
    virtual void wait(u64 timeout = ~0ull) = 0;
    
    virtual void reset() = 0;
    
    virtual bool isSignaled() const = 0;
    
    virtual void* getNativeHandle() const = 0;
};

}
