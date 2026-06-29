#include <gfx/hal/opengl/GLBuffer.h>
#include <glad/gl.h>

namespace gfx {

GLBuffer::GLBuffer(const BufferDesc& desc) : m_desc(desc) {
    glGenBuffers(1, &m_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    
    unsigned int glUsage = GL_STATIC_DRAW;
    if ((desc.usage & BufferUsage::STORAGE) != static_cast<BufferUsage>(0)) {
        glUsage = GL_DYNAMIC_DRAW;
    }
    
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(desc.size), desc.initialData, glUsage);
}

GLBuffer::~GLBuffer() {
    if (m_buffer != 0) {
        glDeleteBuffers(1, &m_buffer);
    }
}

const BufferDesc& GLBuffer::getDesc() const {
    return m_desc;
}

void* GLBuffer::map(u64 offset, u64 size) {
    (void)offset;
    (void)size;
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    m_mappedPtr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    return m_mappedPtr;
}

void GLBuffer::unmap() {
    if (m_mappedPtr) {
        glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
        glUnmapBuffer(GL_ARRAY_BUFFER);
        m_mappedPtr = nullptr;
    }
}

void GLBuffer::update(u64 offset, u64 size, const void* data) {
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLintptr>(offset), 
                    static_cast<GLsizeiptr>(size), data);
}

void* GLBuffer::getNativeHandle() const {
    return reinterpret_cast<void*>(m_buffer);
}

}
