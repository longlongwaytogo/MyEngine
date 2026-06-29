#include <gfx/hal/opengl/GLFramebuffer.h>
#include <glad/gl.h>

namespace gfx {

GLFramebuffer::GLFramebuffer(const FramebufferDesc& desc) : m_desc(desc) {
    glGenFramebuffers(1, &m_fbo);
}

GLFramebuffer::~GLFramebuffer() {
    if (m_fbo != 0) {
        glDeleteFramebuffers(1, &m_fbo);
    }
}

const FramebufferDesc& GLFramebuffer::getDesc() const {
    return m_desc;
}

void* GLFramebuffer::getNativeHandle() const {
    return reinterpret_cast<void*>(m_fbo);
}

}
