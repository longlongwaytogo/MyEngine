#pragma once

#include <gfx/core/types.h>

namespace gfx {

class Semaphore {
public:
    virtual ~Semaphore() = default;
    
    virtual void* getNativeHandle() const = 0;
};

}
