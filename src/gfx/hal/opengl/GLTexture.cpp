#include <gfx/hal/opengl/GLTexture.h>
#include <glad/gl.h>

namespace gfx {

GLTexture::GLTexture(const TextureDesc& desc) : m_desc(desc) {
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

GLTexture::~GLTexture() {
    if (m_texture != 0) {
        glDeleteTextures(1, &m_texture);
    }
}

const TextureDesc& GLTexture::getDesc() const {
    return m_desc;
}

void* GLTexture::getNativeHandle() const {
    return reinterpret_cast<void*>(m_texture);
}

}
