#include <gfx/hal/opengl/GLRenderPass.h>

namespace gfx {

GLRenderPass::GLRenderPass(const RenderPassDesc& desc) : m_desc(desc) {
}

GLRenderPass::~GLRenderPass() {
}

const RenderPassDesc& GLRenderPass::getDesc() const {
    return m_desc;
}

void* GLRenderPass::getNativeHandle() const {
    return nullptr;
}

}
