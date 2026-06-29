#pragma once

#include <gfx/hal/Semaphore.h>

namespace gfx {

class GLSemaphore : public Semaphore {
public:
    GLSemaphore();
    ~GLSemaphore() override;
    
    void* getNativeHandle() const override;
};

}
