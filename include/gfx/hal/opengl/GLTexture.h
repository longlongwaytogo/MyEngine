#pragma once

#include <gfx/hal/Texture.h>

namespace gfx {

class GLTexture : public Texture {
public:
    GLTexture(const TextureDesc& desc);
    ~GLTexture() override;
    
    const TextureDesc& getDesc() const override;
    void* getNativeHandle() const override;
    
private:
    TextureDesc m_desc;
    unsigned int m_texture = 0;
};

}
