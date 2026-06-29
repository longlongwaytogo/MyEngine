#include <gfx/hal/opengl/GLFence.h>

namespace gfx {

GLFence::GLFence(bool signaled) : m_signaled(signaled) {
}

GLFence::~GLFence() {
}

void GLFence::wait(u64 timeout) {
    (void)timeout;
    m_signaled = true;
}

bool GLFence::isSignaled() const {
    return m_signaled;
}

void GLFence::reset() {
    m_signaled = false;
}

void* GLFence::getNativeHandle() const {
    return nullptr;
}

}