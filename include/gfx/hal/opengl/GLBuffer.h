#pragma once

#include <gfx/hal/Buffer.h>

namespace gfx {

class GLBuffer : public Buffer {
public:
    GLBuffer(const BufferDesc& desc);
    ~GLBuffer() override;
    
    const BufferDesc& getDesc() const override;
    void* map(u64 offset, u64 size) override;
    void unmap() override;
    void update(u64 offset, u64 size, const void* data) override;
    void* getNativeHandle() const override;
    
private:
    BufferDesc m_desc;
    unsigned int m_buffer = 0;
    void* m_mappedPtr = nullptr;
};

}
